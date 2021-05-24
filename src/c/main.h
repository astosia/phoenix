#pragma once
#include <pebble.h>
#define SETTINGS_KEY 1
// A structure containing our settings
typedef struct ClaySettings {
  GColor Back1Color;
  GColor Text1Color;
  GColor Text2Color;
  GColor Text3Color;
  GColor Text4Color;
  GColor Text5Color;
  GColor Text7Color;
  GColor HourColor;
  GColor MinColor;
  GColor HourColorN;
  GColor MinColorN;
  GColor Back1ColorN;
  GColor Text1ColorN;
  GColor Text2ColorN;
  GColor Text3ColorN;
  GColor Text4ColorN;
  GColor Text5ColorN;
  GColor Text7ColorN;
  int WeatherUnit;
  char* WindUnit;
  int UpSlider;
  char* WeatherTemp;
  char* TempFore;
  bool NightTheme;
  bool HealthOff;
  bool AddZero12h;
  bool RemoveZero24h;
} __attribute__((__packed__)) ClaySettings;
