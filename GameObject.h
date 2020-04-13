#pragma once

#include "CappedValue.h"
#include "Sprite.h"
#include "Movement.h"


enum class GameObjectType
{
  Terra_Spaceship,
  Terra_Missile,
  Alien_Medium,
  Alien_Heavy,
  Alien_Nimble
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
  Movement m_movement;
  Rect m_movementLimitation;
  bool m_isDespawnPending;
  bool m_collided{false};
  CappedValue<int> m_hp;
  int m_impact;
  GameObjectType m_type;
  Faction m_faction;

  void Move(int dx, int dy)
  {
    m_sprite.SetDirIsUp(dy <= 0);

    if(m_movementLimitation.h > 0)
    {
      Rect newPosition = m_sprite.GetPositionAfterMove(dx, dy);

      if(newPosition.IsColliding(m_movementLimitation))
      {
        m_sprite.Move(dx, dy);
      }
    }
    else
    {
      m_sprite.Move(dx, dy);
    }
  }
public:
  GameObject(Sprite s, int hp, int impact, Movement m, GameObjectType type, Faction faction): m_sprite{s}, m_hp(hp), m_impact(impact), m_movement(m), m_type(type), m_faction(faction), m_isDespawnPending(false), m_movementLimitation{0, 0, 0, 0}{}
  
  void Update(int dt)
  {
    m_collided = false;
    m_sprite.Hide();
    m_movement.Update(); 
    Move(m_movement.GetDX(), m_movement.GetDY());
    m_sprite.Draw();
  }

  void SetMovement(Movement m)
  {
    m_movement = m;
  }

  Movement& GetMovement()
  {
    return m_movement;
  }

  void LimitMovement(Rect limit)
  {
    m_movementLimitation = limit;
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

  int GetScore() const
  {
    return m_hp.GetMax();
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
