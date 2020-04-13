#include <M5StickC.h>

#include "resources.h"
#include "geometry.h"
#include "World.h"
#include "BatteryData.h"
#include "Countdown.h"

// m5stickC has 80x160 screen
Rect g_screen{0, 0, 80, 160};
World g_world;
const int k_maxEnemies = 5;

Countdown g_inputDelay;

const int fps = 25;

double delta, old_time, current_time;

void renderWidgets()
{
  M5.Lcd.setTextColor(TFT_WHITE, BCKGRDCOL);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(3, 3, 1);
  M5.Lcd.printf("%d ", g_world.GetScore());

  BatteryData batLevel = BatteryData::GetBatteryLevel();
  int color = TFT_WHITE;
  int bgcolor = BCKGRDCOL;
  
  if(!batLevel.isCharging)
  {
    int bgcolor = batLevel.isCharging ? TFT_GREEN : BCKGRDCOL;
    if(batLevel.lvl > 60)
    {
      color = TFT_GREEN;
    }
    else if(batLevel.lvl > 40)
    {
      color = TFT_YELLOW;
    }
    else if(batLevel.lvl <= 20)
    {
      bgcolor = TFT_RED;
    }
    else
    {
      color = TFT_RED;
    }
  }
  else
  {
    color = BCKGRDCOL;
    bgcolor = TFT_GREEN;
  }
  
  if(bgcolor == color)
  {
    color = BCKGRDCOL;
  }
  
  M5.Lcd.setTextColor(color, bgcolor);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor( g_screen.w - 25, 3, 1);
  if(batLevel.lvl > 20)
  {
    M5.Lcd.printf(" %d%%", batLevel.lvl);
  }
  else
  {
    M5.Lcd.printf("low");
  }
}

void processInput()
{
  float accelX = 0.f, accelY = 0.f, accelZ = 0.f;
  M5.IMU.getAccelData(&accelX,&accelY,&accelZ);

  const int k_speedY = 2;
  const int k_sensitivity = 10;
  
  int impulse = accelX * k_sensitivity;
  if(impulse < 0)
  {
    impulse = k_speedY;
  }
  else if(impulse > 0)
  {
    impulse = -k_speedY;
  }
  g_world.AddImpulseToMC(impulse, 0);
  
  if(g_inputDelay.Update(delta))
  {
    if(digitalRead(BUTTON_B_PIN) == LOW)
    {
      g_world.SpawnMissileAttack(5);
      g_inputDelay.Start(1000);
    }

    if(digitalRead(M5_BUTTON_HOME) == LOW)
    {
      g_world.SpawnMissileAttack(1);
      g_inputDelay.Start(300);
    }
  }
}



void setup()
{
  // put your setup code here, to run once:
  
  M5.begin();
 
  pinMode(M5_BUTTON_HOME, INPUT);
  pinMode(BUTTON_B_PIN, INPUT);
  
  M5.IMU.Init();

  M5.Lcd.fillScreen(BCKGRDCOL);
  current_time = millis();
  
  g_world.SetScreen(g_screen);
  g_world.Populate(k_maxEnemies);
}

void loop()
{
  // put your main code here, to run repeatedly:
  
  processInput();
  
  if(g_world.Update(delta) == World::State::Over)
  {
    M5.Lcd.fillScreen(BCKGRDCOL);
    g_world.Populate(k_maxEnemies);
  }
  
  renderWidgets();
  
  old_time = current_time;
  current_time = millis();
  delta = current_time - old_time;
  
  int frameDelay = (1000 / fps) - delta;
  if(frameDelay > 0)
  {
    delay(frameDelay);
  }
}
