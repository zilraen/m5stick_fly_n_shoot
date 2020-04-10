#pragma once

template<class T> class CappedValue
{
public:
  CappedValue(): m_value(T()), m_valueMax(T()), m_valueMin(T()){}
  CappedValue(T value): m_value(value), m_valueMax(value), m_valueMin(T()){}
  CappedValue(T value, T valueMax, T valueMin): m_value(value), m_valueMax(valueMax), m_valueMin(valueMin){}

  const T& GetMax() const
  {
    return m_valueMax;
  }

  const T& GetMin() const
  {
    return m_valueMin;
  }

  const T& Get() const
  {
    return m_value;
  }

  void SetMax(T value)
  {
    m_valueMax = value;
    Set(m_value);
  }

  void SetMin(T value)
  {
    m_valueMin = value;
    Set(m_value);
  }
  
  void Set(T value)
  {
    if(value <= m_valueMax)
    {
      if(value >= m_valueMin)
      {
        m_value = value;
      }
      else
      {
        m_value = m_valueMin;
      }
    }
    else
    {
      m_value = m_valueMax;
    }
  }

  void Inc(T value)
  {
    Set(m_value + value);
  }

  void Dec(T value)
  {
    Inc(-value);
  }
  
  T& operator=(T value)
  {
    Set(value);
    return m_value;
  }

  bool operator==(T value) const
  {
    return m_value == value;
  }

  bool operator>(T value) const
  {
    return m_value > value;
  }

  bool operator<(T value) const
  {
    return m_value < value;
  }

  bool operator>=(T value) const
  {
    return m_value >= value;
  }

  bool operator<=(T value) const
  {
    return m_value <= value;
  }
  
private:
  T m_value;
  T m_valueMax;
  T m_valueMin;
};
