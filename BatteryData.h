#pragma once

#include <M5StickC.h>

struct BatteryData
{
  void SetLevel(int _lvl){ lvl = _lvl > 100 ? 100 : _lvl; }
  static BatteryData GetBatteryLevel()
  {
    BatteryData res;
    float c = M5.Axp.GetVapsData() * 1.4 / 1000;
    float b = M5.Axp.GetVbatData() * 1.1 / 1000;
  
    res.SetLevel(((b - 3.0) / 1.2) * 100);
    res.isCharging = (c >= 4.5);
  
    return res;
  }
  
  int lvl{0};
  bool isCharging{false};
};
