#include "ShockCollar.hpp"
#include "ShockCollarRF.hpp"

#include <stringdb.h>

#define PATTERN_SIZE 16
#define PATTERN_COUNT 13

namespace 
{
    static const uint8_t s_patterns[PATTERN_COUNT][PATTERN_SIZE] = {
        { 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        { 15,  0,  0,  0,  0,  0,  0,  0, 15,  0,  0,  0,  0,  0,  0,  0 },
        { 15,  0,  0,  0, 15,  0,  0,  0, 15,  0,  0,  0, 15,  0,  0,  0 },
        { 15,  0, 15,  0, 15,  0, 15,  0, 15,  0, 15,  0, 15,  0, 15,  0 },
        { 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 },
        {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15 },
        {  1,  3,  5,  7,  9, 11, 13, 15,  1,  3,  5,  7,  9, 11, 13, 15 },
        {  3,  7, 11, 15,  3,  7, 11, 15,  3,  7, 11, 15,  3,  7, 11, 15 },
        {  0,  1,  2,  3,  4,  5,  6,  7,  0,  0,  0,  0,  0,  0,  0,  0 },
        {  1,  3,  5,  7,  9, 11, 13, 15,  0,  0,  0,  0,  0,  0,  0,  0 },
        {  3,  7, 11, 15,  3,  7, 11, 15,  0,  0,  0,  0,  0,  0,  0,  0 },
        {  0,  1,  2,  3,  4,  5,  6,  7,  1,  3,  5,  7,  9, 11, 13, 15 },
        {  1,  3,  5,  7,  9, 11, 13, 15,  3,  7, 11, 15,  3,  7, 11, 15 },
    };

    void patternString(char* out, int patternIdx, int patternTime)
    {
        for(int i = 0; i < PATTERN_SIZE; ++i)
        {
            const uint8_t val = s_patterns[patternIdx][i];
            if(i == patternTime)
            {
                out[i] = 'X';
            }
            else
            {
                switch(val)
                {
                    case 0x0: out[i] = '.';
                    case 0x1: out[i] = '1';
                    case 0x2: out[i] = '2';
                    case 0x3: out[i] = '3';
                    case 0x4: out[i] = '4';
                    case 0x5: out[i] = '5';
                    case 0x6: out[i] = '6';
                    case 0x7: out[i] = '7';
                    case 0x8: out[i] = '8';
                    case 0x9: out[i] = '9';
                    case 0xa: out[i] = 'A';
                    case 0xb: out[i] = 'B';
                    case 0xc: out[i] = 'C';
                    case 0xd: out[i] = 'D';
                    case 0xe: out[i] = 'E';
                    case 0xf: out[i] = 'F';
                }
            }
        }
    }
}


void ShockCollar::begin()
{    
    ShockCollarSetup();

    mode_ = TYPE_VIBRATE;

    App::begin();
}


void ShockCollar::key_event(uint8_t key)
{
    ShockCollarMsg msg = {};

    switch( key )
    {
        case APP_KEY_SCROLL_CLOCKWISE:
            if( maxPower_ < 99 )
                ++maxPower_;
            redraw_ = true;
            break;

        case APP_KEY_SCROLL_ANTICLOCKWISE:
            if( maxPower_ > 0 )
                --maxPower_;
            redraw_ = true;
            break;

        case APP_KEY_OK:
            if( mode_ == TYPE_VIBRATE )
            {
                mode_ = TYPE_SHOCK;
                maxPower_ = 0; // Reset power to zero when switching to shock.

                // Send a couple of beeps.
                msg = BuildShockCollarMsg( RemoteSecret, 0, TYPE_BEEP, 0 );
                TransmitShockCollarMsg( msg );
                TransmitShockCollarMsg( msg );
            }
            else if( mode_ == TYPE_SHOCK )
            {
                mode_ = TYPE_VIBRATE;
            }
            else if( mode_ == TYPE_BEEP )
            {
                mode_ = TYPE_VIBRATE;
            }
            redraw_ = true;
            break;

        case APP_KEY_FORWARD:
            if( patternIdx_ < PATTERN_COUNT )
                ++patternIdx_;
            else
                patternIdx_ = 0;
            redraw_ = true;
            break;

        case APP_KEY_BACK:
            if( patternIdx_ > 0 )
                --patternIdx_;
            else
                patternIdx_ = PATTERN_COUNT - 1;
            redraw_ = true;
            break;

        case APP_KEY_PLAY:
            runPattern_ = !runPattern_;
            frameIdx_ = 0;
            redraw_ = true;

            // Send a couple of beeps.
            msg = BuildShockCollarMsg( RemoteSecret, 0, TYPE_BEEP, 0 );
            TransmitShockCollarMsg( msg );
            TransmitShockCollarMsg( msg );
            break;

        case APP_KEY_MENU:
            App::manager_end();
            break;
    }
}

void ShockCollar::draw(Adafruit_GFX &gfx)
{
    char buf[100];

    const uint8_t patternTime = runPattern_ ? ( frameIdx_ / 8 ) % PATTERN_SIZE : PATTERN_SIZE;
    const bool sendCommand = frameIdx_ % 8 == 0;

    // redraw on when required.
    if(redraw_ || sendCommand)
    {
        BpodTitleBar::draw(gfx, STRING(STRING_SHOCK_COLLAR));
        gfx.fillRect(0, BpodTitleBar::view_y(gfx), gfx.width(), BpodTitleBar::view_height(gfx), 0xffff);

        gfx.setTextColor(0x0000);
        gfx.setTextSize(1, 1);

        const char* modes[] = {
            "",
            "Shock",
            "Vibrate",
            "Beep",
        };

        int16_t x = gfx.getCursorX();
        int16_t y = gfx.getCursorY();

        y += 16;
        gfx.setCursor(x, y);
        gfx.printf("Max Power: %u\n", maxPower_);

        y += 16;
        sprintf(buf, "Pattern: " );
        patternString(buf+9, patternIdx_, patternTime);
        gfx.setCursor(x, y);
        gfx.print(buf);

        y += 16;
        sprintf(buf, "Mode: %s\n", modes[mode_]);
        gfx.setCursor(x, y);
        gfx.printf("Mode: %s\n", modes[mode_]); 

        y += 16;
        gfx.print(runPattern_ ? "ON\n" : "OFF");

        redraw_ = false;
    }

    // do shock pattern.
    if( runPattern_ && patternTime < PATTERN_SIZE )
    {
        const uint8_t patternValue = s_patterns[patternIdx_][patternTime];
        const uint8_t power = calculatePower( patternValue );

        if( sendCommand )
        {
            if( power > 0 )
            {
                ShockCollarMsg msg = BuildShockCollarMsg( RemoteSecret, 0, mode_, power );
                TransmitShockCollarMsg( msg );
            }
        }

        frameIdx_++;
    }
}

uint8_t ShockCollar::calculatePower( uint8_t power ) const 
{
    uint16_t actualPower = ((uint16_t)power * (uint16_t)maxPower_ * 6) / 99;
    if(actualPower > maxPower_)
        actualPower = maxPower_;
    return actualPower;
}
