#pragma once

#include <list>
#include <memory>

#include "GameObject.h"
#include "VFX.h"

class World
{
public:
  GameObjectType GetAlienType()
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

  bool AreEnemies(std::shared_ptr<GameObject> go1, std::shared_ptr<GameObject> go2)
  {
    Faction f1 = go1->GetFaction();
    Faction f2 = go2->GetFaction();

    // different factions or any faction is NoFaction: no need to check another one in this case.
    return (f1 != f2) || (f1 == Faction::NoFaction);
  }

  std::shared_ptr<GameObject> SpawnAlienShip(int x, int y)
  {
    GameObjectType t = GetAlienType();
    return SpawnGameObject(t, x, y);
  }

  std::shared_ptr<GameObject> SpawnTerraShip(int x, int y)
  {
    GameObjectType t = GameObjectType::Terra_Spaceship;
    return SpawnGameObject(t, x, y);
  }

  std::shared_ptr<GameObject> SpawnTerraMissile(int x, int y)
  {
    GameObjectType t = GameObjectType::Terra_Missile;
    return SpawnGameObject(t, x, y);
  }

  std::shared_ptr<VFX> SpawnVFXExplosion(int x, int y)
  {
    return std::make_shared<VFX>(Rect{x, y, k_frame_size, k_frame_size}, vfx_explosion, 0, false);
  }

  std::shared_ptr<VFX> SpawnVFXStar(int x, int y)
  {
    int framesDelay = random(3, 6) * 10;
    return std::make_shared<VFX>(Rect{x, y, k_frame_size, k_frame_size}, vfx_star, framesDelay, true);
  }
  
  std::shared_ptr<GameObject> SpawnGameObject(GameObjectType type, int x, int y)
  {
    switch(type)
    {
      case GameObjectType::Terra_Spaceship:
        return std::make_shared<GameObject>(Sprite{x, y, k_frame_size, k_frame_size, px_spaceship, BCKGRDCOL}, 1, 1, Movement{2}, type, Faction::Terra);
        break;
      case GameObjectType::Alien_Medium:
        return std::make_shared<GameObject>(Sprite{x, y, k_frame_size, k_frame_size, px_alien0, BCKGRDCOL}, 2, 1, Movement{2}, type, Faction::Alien);
        break;
      case GameObjectType::Alien_Heavy:
        return std::make_shared<GameObject>(Sprite{x, y, k_frame_size, k_frame_size, px_alien1, BCKGRDCOL}, 3, 2, Movement{1}, type, Faction::Alien);
        break;
      case GameObjectType::Alien_Nimble:
        return std::make_shared<GameObject>(Sprite{x, y, k_frame_size, k_frame_size, px_alien2, BCKGRDCOL}, 1, 0, Movement{3}, type, Faction::Alien);
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
    int ix = random(1, 2);
    int iy = random(1, 5);
    if(random(2))
    {
      go = SpawnAlienShip(x, y);
      go->GetMovement().AddImpulse(ix, iy);
    }
    else
    {
      y = m_screen.h - 1;
      ix = 1;
      iy = random(-2, -5);
      
      go = SpawnTerraMissile(x, y);
      go->GetMovement().AddImpulse(ix, iy);
    }

    m_gameobjects.push_back(go);
  }
  
  void Populate(unsigned amount)
  {    
    for(int i = 0; i < amount; ++i)
    {
      SpawnRandomShip();
    }

    for(int i = 0; i < 4; ++i)
    {
      m_vfxBackLayer.push_back(SpawnVFXStar(random(m_screen.w - k_frame_size), random(10, m_screen.h - k_frame_size)));
    }

    if(m_mc == nullptr)
    {
      m_mc = SpawnGameObject(GameObjectType::Terra_Spaceship, m_screen.w / 2, m_screen.h - 2 * k_frame_size);
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

  void Update(int dt)
  {
    ProcessVFXBackLayer();
    
    for (auto go: m_gameobjects)
    {
      go->Update(dt);
    }
   
    ProcessCollisions();
    
    ProcessVFXFrontLayer();
  }
  
private:
  std::list<std::shared_ptr<GameObject>> m_gameobjects;
  std::shared_ptr<GameObject> m_mc;

  std::list<std::shared_ptr<VFX>> m_vfxFrontLayer;
  std::list<std::shared_ptr<VFX>> m_vfxBackLayer;
  Rect m_screen;
  unsigned m_score{0};

  void ProcessCollisions()
  {
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
      if(!go->IsColliding(m_screen))
      {
        go->SetDespawnPending();
      }
    }

    for(auto i = m_gameobjects.begin(); i != m_gameobjects.end();)
    {
      if ((*i)->IsDespawnPending())
      {
         (*i)->OnDespawn();
         
         m_vfxFrontLayer.push_back(SpawnVFXExplosion((*i)->GetRect().x,(*i)->GetRect().y));
         
         if(AreEnemies(m_mc, *i) && ((*i)->GetHP() <= 0))
         {
           m_score += (*i)->GetScore();
         }
         
         i = m_gameobjects.erase(i);

         SpawnRandomShip();
       }
       else
       {
         ++i;
       } 
    }
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
