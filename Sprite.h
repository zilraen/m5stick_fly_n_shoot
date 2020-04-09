#pragma once

#include <M5StickC.h>

#include "geometry.h"

// ---------------
// draw pixel
// ---------------
// faster drawPixel method by inlining calls and using setAddrWindow and pushColor
// using macro to force inlining
#define DRAW_PIXEL(a, b, c) M5.Lcd.setAddrWindow(a, b, a, b); M5.Lcd.pushColor(c)

class Sprite
{
  Rect m_rect;
  unsigned* m_pixels;
  unsigned m_bgColor;
  bool m_dirIsUp{true};
  
public:
  Sprite(int _x, int _y, unsigned _w, unsigned _h, unsigned* _pixels, unsigned _bgColor): m_rect(_x, _y, _w, _h), m_pixels(_pixels), m_bgColor(_bgColor){}
  
  void Draw(int _x, int _y)
  {
    m_rect.x = _x;
    m_rect.y = _y;
    Draw();
  }

  void Draw()
  {
    for(int y = m_rect.y; y < m_rect.y + m_rect.h; ++y)
    {
      for(int x = m_rect.x; x < m_rect.x + m_rect.w; ++x)
      {
        if(x >= 0 && y >= 0)
        {
          int pixCoord = 0;
          if(GetDirIsUp())
          {
            pixCoord = (y - m_rect.y) * m_rect.w + (x - m_rect.x);
          }
          else
          {
            pixCoord = ((m_rect.h - 1) - (y - m_rect.y)) * m_rect.w + (x - m_rect.x);
          }
          DRAW_PIXEL(x, y, m_pixels[pixCoord]);
        }
      }
    }
  }

  void Hide()
  {
    for(int y = m_rect.y; y < m_rect.y + m_rect.h; ++y)
    {
      for(int x = m_rect.x; x < m_rect.x + m_rect.w; ++x)
      {
        DRAW_PIXEL(x, y, m_bgColor);
      }
    }
  }

  void SetDirIsUp(bool val)
  {
    m_dirIsUp = val;
  }

  bool GetDirIsUp()
  {
    return m_dirIsUp;
  }
  
  void Move(int dx, int dy)
  {
    m_rect.x += dx;
    m_rect.y += dy;
  }

  const Rect& GetRect() const
  {
    return m_rect;
  }
};
