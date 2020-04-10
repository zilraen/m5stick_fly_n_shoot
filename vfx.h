#pragma once

#include <vector>

#include "Sprite.h"

class VFX
{
public:
  VFX(Rect rect, std::vector<unsigned*> frames, int delayFrames, bool loop): m_rect(rect), m_frames(frames), m_delayFrames(delayFrames), m_delayFramesCount(0), m_loop(loop)
  {
    m_currentFrame = m_frames.begin();
  }

  void Draw()
  {
    if(!IsDespawnPending())
    {
      for(int y = m_rect.y; y < m_rect.y + m_rect.h; ++y)
      {
        for(int x = m_rect.x; x < m_rect.x + m_rect.w; ++x)
        {
          if(x >= 0 && y >= 0)
          {
            int pixCoord = 0;
            pixCoord = (y - m_rect.y) * m_rect.w + (x - m_rect.x);
  
            DRAW_PIXEL(x, y, (*m_currentFrame)[pixCoord]);
          }
        }
      }

      AdvanceFrame();
    }
  }

  bool IsDespawnPending() const
  {
    return m_currentFrame == m_frames.end();
  }

  void SetDespawnPending()
  {
    m_currentFrame = m_frames.end();
  }

private:
  std::vector<unsigned*> m_frames;
  std::vector<unsigned*>::iterator m_currentFrame;
  Rect m_rect;
  bool m_loop;
  int m_delayFrames;
  int m_delayFramesCount;

  void AdvanceFrame()
  {
    if(m_delayFramesCount >= m_delayFrames)
    {
      ++m_currentFrame;
      if(m_loop && (m_currentFrame == m_frames.end()))
      {
        m_currentFrame = m_frames.begin();
      }
      
      m_delayFramesCount = 0;
    }
    else
    {
      ++m_delayFramesCount;
    }
  }
};
