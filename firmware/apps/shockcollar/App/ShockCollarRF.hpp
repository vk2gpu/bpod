#pragma once

#include <cstdint>

static const uint32_t RemoteSecret = 0b0101111110011001;

#define TYPE_SHOCK 1
#define TYPE_VIBRATE 2
#define TYPE_BEEP 3

struct ShockCollarTiming
{
  int PreambleHighTime = 1370;
  int PreambleTotalTime = 2160;

  int BitOneHighTime = 725;
  int BitZeroHighTime = 225;
  int BitTotalTime = 1020;

  bool UseHackyRFSwitch = true;
  int PreSleepTime = 50000;
  int PostSleepTime = 50000;
};

struct ShockCollarMsg
{
  uint8_t bytes[5];
};

void ShockCollarSetup( ShockCollarTiming timing = ShockCollarTiming{} );
ShockCollarMsg BuildShockCollarMsg( uint32_t secret, uint32_t channel, uint32_t type, uint32_t power );
void TransmitShockCollarMsg( ShockCollarMsg msg );
