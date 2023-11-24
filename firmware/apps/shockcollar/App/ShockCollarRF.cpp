#include "ShockCollarRF.hpp"

#include <Arduino.h>

const int PinTX = 5;

static ShockCollarTiming s_timing = {};

ShockCollarMsg BuildShockCollarMsg( uint32_t secret, uint32_t channel, uint32_t type, uint32_t power )
{
  ShockCollarMsg msg = {};

  msg.bytes[0] = secret >> 8;
  msg.bytes[1] = secret & 0xff;
  msg.bytes[2] = (channel & 0x7) << 4 | ( type & 0x7);
  msg.bytes[3] = power > 99 ? 99 : power;
  msg.bytes[4] = msg.bytes[0] + msg.bytes[1] + msg.bytes[2] + msg.bytes[3];
  return msg;
}

static void TransmitSymbol(int highTime, int totalTime )
{
  unsigned long stopwatch = 0;

  stopwatch = micros();
  digitalWrite(PinTX, HIGH);
  while( ( micros() - stopwatch ) < highTime );
  digitalWrite(PinTX, LOW);
  while( ( micros() - stopwatch ) < totalTime );
}

static void TransmitByte( uint8_t byte )
{
  for(int i = 7; i >= 0; --i )
  {
    if( byte & (1 << i) )
      TransmitSymbol( s_timing.BitOneHighTime, s_timing.BitTotalTime );
    else
      TransmitSymbol( s_timing.BitZeroHighTime, s_timing.BitTotalTime );
  }
}

void TransmitShockCollarMsg( ShockCollarMsg msg )
{
  // Hacky switch it off and on again as there was some weird timing issues...
  if( s_timing.UseHackyRFSwitch )
  {
    delayMicroseconds( s_timing.PreSleepTime );
    digitalWrite(PinTX, HIGH);
    digitalWrite(PinTX, LOW);
    delayMicroseconds( s_timing.PostSleepTime );
  }
  
  // Preamble.
  TransmitSymbol( s_timing.PreambleHighTime, s_timing.PreambleTotalTime );

  // Payload.
  for(int i = 0; i < 5; ++i )
  {
    TransmitByte( msg.bytes[i] );
  }

  // End of message.
  TransmitSymbol( s_timing.BitZeroHighTime, s_timing.BitTotalTime );
  TransmitSymbol( s_timing.BitZeroHighTime, s_timing.BitTotalTime );
  TransmitSymbol( s_timing.BitZeroHighTime, s_timing.BitTotalTime );

  digitalWrite(PinTX, LOW);
}

void ShockCollarSetup( ShockCollarTiming timing )
{
  s_timing = timing;

  // Validate timing.
  if( s_timing.BitOneHighTime == 0 )
  {
    s_timing.BitOneHighTime = s_timing.BitTotalTime - s_timing.BitZeroHighTime;
  }
  else if( s_timing.BitZeroHighTime == 0 )
  {
    s_timing.BitZeroHighTime = s_timing.BitTotalTime - s_timing.BitOneHighTime;
  }
  else if( s_timing.BitTotalTime == 0 )
  {
    s_timing.BitTotalTime = s_timing.BitOneHighTime + s_timing.BitZeroHighTime;
  }

  pinMode(PinTX, OUTPUT);
}
