#include "ShockCollarRF.hpp"

#include <Arduino.h>

const int PinTX = 5;

const int PreambleHighTime = 1370;
const int PreambleTotalTime = 2160;

const int BitOneHighTime = 725;
const int BitZeroHighTime = 225;
const int BitTotalTime = 1020;

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
      TransmitSymbol( BitOneHighTime, BitTotalTime );
    else
      TransmitSymbol( BitZeroHighTime, BitTotalTime );
  }
}

void TransmitShockCollarMsg( ShockCollarMsg msg )
{
  // Hacky switch it off and on again as there was some weird timing issues...
  delayMicroseconds( 50000 );
  digitalWrite(PinTX, HIGH);
  digitalWrite(PinTX, LOW);
  delayMicroseconds( 50000 );

  // Preamble.
  TransmitSymbol( PreambleHighTime, PreambleTotalTime );

  // Payload.
  for(int i = 0; i < 5; ++i )
  {
    TransmitByte( msg.bytes[i] );
  }

  // End of message.
  TransmitSymbol( BitZeroHighTime, BitTotalTime );
  TransmitSymbol( BitZeroHighTime, BitTotalTime );
  TransmitSymbol( BitZeroHighTime, BitTotalTime );

  digitalWrite(PinTX, LOW);
}

void ShockCollarSetup()
{
  pinMode(PinTX, OUTPUT);
}
