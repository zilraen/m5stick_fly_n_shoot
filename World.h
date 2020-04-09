#pragma once

#include <list>
#include <memory>

#include "GameObject.h"

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
  
  std::shared_ptr<GameObject> SpawnGameObject(GameObjectType type, int x, int y)
  {
    switch(type)
    {
      case GameObjectType::Terra_Spaceship:
        return std::make_shared<GameObject>(Sprite{x, y, 8, 8, px_spaceship, BCKGRDCOL}, 1, 1, type, Faction::Terra);
        break;
      case GameObjectType::Alien_Medium:
        return std::make_shared<GameObject>(Sprite{x, y, 8, 8, px_alien0, BCKGRDCOL}, 2, 1, type, Faction::Alien);
        break;
      case GameObjectType::Alien_Heavy:
        return std::make_shared<GameObject>(Sprite{x, y, 8, 8, px_alien1, BCKGRDCOL}, 3, 2, type, Faction::Alien);
        break;
      case GameObjectType::Alien_Nimble:
        return std::make_shared<GameObject>(Sprite{x, y, 8, 8, px_alien2, BCKGRDCOL}, 1, 0, type, Faction::Alien);
        break;
      case GameObjectType::Terra_Missile:
        return std::make_shared<GameObject>(Sprite{x, y, 8, 8, px_missile, BCKGRDCOL}, 1, 2, type, Faction::Terra);
        break;
    }
  }

  void SpawnRandomShip()
  {
    std::shared_ptr<GameObject> go;
    
    int x = random(m_screen.w);
    int y = 1;
    if(random(2))
    {
      go = SpawnAlienShip(x, y);
      go->SetSpeed(Point{1, 2});
    }
    else
    {
      y = m_screen.h - 1;
      go = SpawnTerraMissile(x, y);
      go->SetSpeed(Point{1, -2});
    }

    m_gameobjects.push_back(go);
  }
  
  void Populate(unsigned amount)
  {
    for(int i = 0; i < amount; ++i)
    {
      SpawnRandomShip();
    }

    if(m_mc == nullptr)
    {
      m_mc = SpawnGameObject(GameObjectType::Terra_Spaceship, m_screen.w / 2, m_screen.h - 2 * 8);
      m_gameobjects.push_back(m_mc);
    }
  }
  
  void SetScreen(Rect screen)
  {
	  m_screen = screen;
  }

  unsigned GetScore()
  {
    return m_score / 2;
  }

  unsigned GetObjectsAmount()
  {
    return m_gameobjects.size();
  }

  void Update(int dt)
  {
  	for (auto go: m_gameobjects)
  	{
      go->Update(dt);
  	}
   
    ProcessCollisions();
  }
  
private:
  std::list<std::shared_ptr<GameObject>> m_gameobjects;
  std::shared_ptr<GameObject> m_mc;
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
         if(AreEnemies(m_mc, *i))
         {
           ++m_score;      
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
};
