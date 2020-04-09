#pragma once

#include "CappedValue.h"
#include "Sprite.h"


enum class GameObjectType
{
  Terra_Spaceship,
  Alien_Medium,
  Alien_Heavy,
  Alien_Nimble,
  Terra_Missile
};

enum class Faction
{
  Terra,
  Alien,
  NoFaction
};

class GameObject
{
  Sprite m_sprite;
  Point m_speed;
  bool m_isDespawnPending;
  bool m_collided{false};
  CappedValue<int> m_hp;
  int m_impact;
  GameObjectType m_type;
  Faction m_faction;

  void Move(int dx, int dy)
  {
    m_sprite.SetDirIsUp(dy <= 0);
    m_sprite.Move(dx, dy);
  }
public:
  GameObject(Sprite s, int hp, int impact, GameObjectType type, Faction faction): m_sprite{s}, m_hp(hp), m_impact(impact), m_type(type), m_faction(faction), m_isDespawnPending(false){}
  
  void Update(int dt)
  {
    m_collided = false;
    m_sprite.Hide();    
    Move(m_speed.x, m_speed.y);
    m_sprite.Draw();
  }

  void SetSpeed(Point s)
  {
    m_speed = s;
  }

  GameObjectType GetType() const
  {
    return m_type;
  }

  Faction GetFaction() const
  {
    return m_faction;
  }

  int GetHP() const
  {
    return m_hp.Get();
  }

  int GetImpact() const
  {
    return m_impact;
  }

  void OnCollision(const GameObject& other)
  {
    if(!m_collided)
    {
      m_hp.Dec(other.GetImpact());
      m_collided = true;
  
      if(m_hp <= 0)
      {
        SetDespawnPending();
      }
    }
  }

  bool IsDespawnPending() const
  {
    return m_isDespawnPending;
  }

  void SetDespawnPending()
  {
    m_isDespawnPending = true;
  }

  void OnDespawn()
  {
    m_sprite.Hide();
  }

  const Rect& GetRect() const
  {
    return m_sprite.GetRect();
  }

  bool IsColliding(const GameObject& other) const
  {
    return IsColliding(other.GetRect());
  }

  bool IsColliding(const Rect& rect) const
  {
    return GetRect().IsColliding(rect);
  }
};
