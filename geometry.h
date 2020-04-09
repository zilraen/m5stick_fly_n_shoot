#pragma once

struct Point
{
  Point(){}
  Point(int _x, int _y): x(_x), y(_y){}
  
  int x{0};
  int y{0};
};

struct Rect: public Point
{
  Rect(){}
  Rect(int _x, int _y, unsigned _w, unsigned _h): x(_x), y(_y), h(_h), w(_w){}

  bool IsPointInside(const Point& p) const
  {
    bool result = (p.x < x + w) && (p.x >= x) && (p.y < y + h) && (p.y >= y);
    return result;
  }
  
  bool IsColliding(const Rect& other) const
  {
    Point points[] = {{x, y}, {x + w - 1, y}, {x, y + h - 1}, {x + w - 1, y + h - 1}};
    Point pointsOther[] = {{other.x, other.y}, {other.x + other.w - 1, other.y}, {other.x, other.y + other.h - 1}, {other.x + other.w - 1, other.y + other.h - 1}};
    
    bool result = false;
    for (int i = 0; i < 4; ++i)
    {
      result |= other.IsPointInside(points[i]);
      result |= IsPointInside(pointsOther[i]);

      if(result) break;
    }
    
    return result;
  }

  int x{0};
  int y{0};
  
  int h{0};
  int w{0};
};
