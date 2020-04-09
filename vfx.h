#pragma once

#include <vector>

#include "Sprite.h"

class VFX
{
public:
  VFX(Rect rect): m_rect(rect); 

private:
  std::vector<Sprite> m_frames;
  Rect m_rect;
};
