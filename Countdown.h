#pragma once

class Countdown
{
public:
  void Start(int count)
  {
    m_count = count;
  }
  
  bool Update(int dt)
  {
    if(m_count > 0)
    {
      m_count -= dt;
    }

    return (m_count <= 0);
  }
  
private:
  int m_count{0};
};
