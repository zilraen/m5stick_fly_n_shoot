#pragma once

#include <list>
#include <memory>

#include "GameObject.h"
#include "VFX.h"

class World
{
public:
  enum class State
  {
    Running,
    Over
  };
  
  bool AreEnemies(std::shared_ptr<GameObject> go1, std::shared_ptr<GameObject> go2)
  {
    Faction f1 = go1->GetFaction();
    Faction f2 = go2->GetFaction();

    // different factions or any faction is NoFaction: no need to check another one in this case.
    return (f1 != f2) || (f1 == Faction::NoFaction);
  }
  
  GameObjectType GetRandomAlienType()
  {
    GameObjectType t;
    int r = random(10);
    if(r < 5)
    {
      t = GameObjectType::Alien_Medium;
    }
    else if(r < 8)
    {
      t = GameObjectType::Alien_Nimble;
    }
    else
    {
      t = GameObjectType::Alien_Heavy;
    }

    return t;
  }

  std::shared_ptr<GameObject> GetAlienShip(int x, int y)
  {
    GameObjectType t = GetRandomAlienType();
    return GetGameObject(t, x, y);
  }

  std::shared_ptr<GameObject> GetTerraShip(int x, int y)
  {
    GameObjectType t = GameObjectType::Terra_Spaceship;
    return GetGameObject(t, x, y);
  }

  std::shared_ptr<GameObject> GetTerraMissile(int x, int y)
  {
    GameObjectType t = GameObjectType::Terra_Missile;
    return GetGameObject(t, x, y);
  }

  std::shared_ptr<VFX> GetVFXExplosion(int x, int y)
  {
    return std::make_shared<VFX>(Rect{x, y, k_frame_size, k_frame_size}, vfx_explosion, 0, false);
  }

  std::shared_ptr<VFX> GetVFXStar(int x, int y)
  {
    int framesDelay = random(3, 6) * 10;
    return std::make_shared<VFX>(Rect{x, y, k_frame_size, k_frame_size}, vfx_star, framesDelay, true);
  }
  
  std::shared_ptr<GameObject> GetGameObject(GameObjectType type, int x, int y)
  {
    switch(type)
    {
      case GameObjectType::Terra_Spaceship:
        return std::make_shared<GameObject>(Sprite{x, y, k_frame_size, k_frame_size, px_spaceship, BCKGRDCOL}, 2, 1, Movement{2}, type, Faction::Terra);
        break;
      case GameObjectType::Alien_Medium:
        return std::make_shared<GameObject>(Sprite{x, y, k_frame_size, k_frame_size, px_alien0, BCKGRDCOL}, 2, 1, Movement{2}, type, Faction::Alien);
        break;
      case GameObjectType::Alien_Heavy:
        return std::make_shared<GameObject>(Sprite{x, y, k_frame_size, k_frame_size, px_alien1, BCKGRDCOL}, 3, 2, Movement{1}, type, Faction::Alien);
        break;
      case GameObjectType::Alien_Nimble:
        return std::make_shared<GameObject>(Sprite{x, y, k_frame_size, k_frame_size, px_alien2, BCKGRDCOL}, 1, 1, Movement{3}, type, Faction::Alien);
        break;
      case GameObjectType::Terra_Missile:
        return std::make_shared<GameObject>(Sprite{x, y, k_frame_size, k_frame_size, px_missile, BCKGRDCOL}, 1, 2, Movement{3}, type, Faction::Terra);
        break;
    }
  }

  void SpawnRandomShip()
  {
    std::shared_ptr<GameObject> go;
    
    int x = random(m_screen.w);
    int y = 1;
    int ix = random(-2, 2);
    int iy = random(1, 5);
    
    go = GetAlienShip(x, y);
    go->GetMovement().AddImpulse(ix, iy);

    m_gameobjects.push_back(go);
  }

  void SpawnMissileAttack(int amount)
  {
    for (int i = 0; i < amount; ++i)
    {
      int x = m_mc->GetRect().x + (i - amount / 2) * k_frame_size;
      int y = m_mc->GetRect().y;
      int ix = 0;
      int iy = -3;

      if(amount > 1)
      {
        // help from Terra!
        y = m_screen.h - 1;
      }
      
      std::shared_ptr<GameObject> go = GetTerraMissile(x, y);
      go->GetMovement().AddImpulse(ix, iy);
      
      m_gameobjects.push_back(go);
    }  
  }

  void AddImpulseToMC(int ix, int iy)
  {
    m_mc->GetMovement().AddImpulse(ix, iy);
  }
  
  void Populate(unsigned amount)
  {   
    m_gameobjects.clear();
    m_vfxFrontLayer.clear();
    m_vfxBackLayer.clear();
    m_score = 0;
     
    for(int i = 0; i < amount; ++i)
    {
      SpawnRandomShip();
    }

    for(int i = 0; i < 4; ++i)
    {
      m_vfxBackLayer.push_back(GetVFXStar(random(m_screen.w - k_frame_size), random(10, m_screen.h - k_frame_size)));
    }

    if(m_mc == nullptr || m_mc->IsDespawnPending())
    {
      m_mc = GetGameObject(GameObjectType::Terra_Spaceship, m_screen.w / 2, m_screen.h - 2 * k_frame_size);
      m_mc->LimitMovement(m_screen);
      
      m_gameobjects.push_back(m_mc);
    }
  }
  
  void SetScreen(Rect screen)
  {
    m_screen = screen;
  }

  unsigned GetScore()
  {
    return m_score;
  }

  unsigned GetObjectsAmount()
  {
    return m_gameobjects.size();
  }

  World::State Update(int dt)
  {
    ProcessVFXBackLayer();
    
    for (auto go: m_gameobjects)
    {
      go->Update(dt);
    }
   
    const bool gameOver = ProcessCollisions();
    
    ProcessVFXFrontLayer();

    return gameOver ? World::State::Over : World::State::Running;
  }
  
private:
  std::list<std::shared_ptr<GameObject>> m_gameobjects;
  std::shared_ptr<GameObject> m_mc;

  std::list<std::shared_ptr<VFX>> m_vfxFrontLayer;
  std::list<std::shared_ptr<VFX>> m_vfxBackLayer;
  Rect m_screen;
  int m_score{0};

  bool ProcessCollisions()
  {
    bool gameOver = false;
    
    for (auto go: m_gameobjects)
    {
      for (auto other: m_gameobjects)
      {
        if((go != other) && go->IsColliding(*other) && AreEnemies(go, other))
        {
          go->OnCollision(*other);
          other->OnCollision(*go);
        }
      }

      // object goes offscreen, despawn it
      if(!go->IsColliding(m_screen) && (go != m_mc))
      {
        go->SetDespawnPending();
      }
    }

    for(auto i = m_gameobjects.begin(); i != m_gameobjects.end();)
    {
      if ((*i)->IsDespawnPending())
      {
         (*i)->OnDespawn();
         
         m_vfxFrontLayer.push_back(GetVFXExplosion((*i)->GetRect().x,(*i)->GetRect().y));
         
         if(AreEnemies(m_mc, *i))
         {
           if((*i)->GetHP() <= 0)
           { 
             m_score += (*i)->GetScore();
           }
           else if((*i)->GetRect().y > m_screen.w)
           {
             m_score -= (*i)->GetScore();
           }
           
           SpawnRandomShip();
         }
         
         i = m_gameobjects.erase(i);
       }
       else
       {
         ++i;
       } 
    }

    if(m_mc->IsDespawnPending())
    {
      gameOver = true;
    }

    return gameOver;
  }

  void ProcessVFXFrontLayer()
  {
    for(auto i = m_vfxFrontLayer.begin(); i != m_vfxFrontLayer.end();)
    {
      (*i)->Draw();
      
      if ((*i)->IsDespawnPending())
      {
         i = m_vfxFrontLayer.erase(i);
       }
       else
       {
         ++i;
       } 
    }
  }

  void ProcessVFXBackLayer()
  {
    for(auto i = m_vfxBackLayer.begin(); i != m_vfxBackLayer.end();)
    {
      (*i)->Draw();
      
      if ((*i)->IsDespawnPending())
      {
         i = m_vfxBackLayer.erase(i);
       }
       else
       {
         ++i;
       } 
    }
  }
};
