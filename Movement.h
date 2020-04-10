#pragma once

#include "CappedValue.h"

class Movement
{
public:
  Movement(){}
  Movement(int maxSpeedAccel): Movement(maxSpeedAccel, maxSpeedAccel){}
  Movement(int maxSpeed, int maxAccel)
  {
    m_speedX.SetMax(maxSpeed);
    m_speedX.SetMin(-maxSpeed);
    m_speedY.SetMax(maxSpeed);
    m_speedY.SetMin(-maxSpeed);

    m_accelX.SetMax(maxAccel);
    m_accelX.SetMin(-maxAccel);
    m_accelY.SetMax(maxAccel);
    m_accelY.SetMin(-maxAccel);
  }
  
  void AddImpulse(int x, int y)
  {
    m_accelX.Inc(x);
    m_accelY.Inc(y);
  }
  
  int GetDX() const
  {
    return m_speedX.Get();
  }
  
  int GetDY() const
  {
    return m_speedY.Get();  
  }
  
  void Update()
  {
    m_speedX.Inc(m_accelX.Get());
    m_accelX.Set(0);
    m_speedY.Inc(m_accelY.Get());
    m_accelY.Set(0);
  }
  
private:
  CappedValue<int> m_speedX;
  CappedValue<int> m_speedY;

  CappedValue<int> m_accelX;
  CappedValue<int> m_accelY;
};
