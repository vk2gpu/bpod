#include "ShockCollar.hpp"
#include "ShockCollarRF.hpp"

#include <stringdb.h>

#include <freertos/task.h>
#include <freertos/queue.h>

#define USE_CORE_FOR_RF         1



#define PATTERN_SIZE 16
#define PATTERN_COUNT 12

namespace 
{
    static const uint8_t s_patterns[PATTERN_COUNT][PATTERN_SIZE] = {
        { 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        { 15,  0,  0,  0,  0,  0,  0,  0, 15,  0,  0,  0,  0,  0,  0,  0 },
        { 15,  0,  0,  0, 15,  0,  0,  0, 15,  0,  0,  0, 15,  0,  0,  0 },
        { 15,  0, 15,  0, 15,  0, 15,  0, 15,  0, 15,  0, 15,  0, 15,  0 },
        { 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 },
        {  0,  1,  0,  3,  0,  5,  0,  7,  0,  9,  0, 11,  0, 13,  0, 15 },
        {  0,  3,  0,  7,  0, 11,  0, 15,  0,  3,  0,  7,  0, 11,  0, 15 },
        {  0,  7,  0, 15,  0,  7,  0, 15,  0,  7,  0, 15,  0,  7,  0, 15 },
        {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11,  0,  0,  0,  0 },
        {  0,  3,  5,  7,  9, 11, 13, 15, 12, 13, 14, 15,  0,  0,  0,  0 },
        {  0,  7, 11, 15,  3,  7, 11, 15, 12, 13, 14, 15,  0,  0,  0,  0 },
        {  0,  3,  0,  7,  0, 11,  0, 15,  0,  7,  0, 15,  0,  7,  0, 15 },
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
                    case 0x0: out[i] = '.'; break;
                    case 0x1: out[i] = '1'; break;
                    case 0x2: out[i] = '2'; break;
                    case 0x3: out[i] = '3'; break;
                    case 0x4: out[i] = '4'; break;
                    case 0x5: out[i] = '5'; break;
                    case 0x6: out[i] = '6'; break;
                    case 0x7: out[i] = '7'; break;
                    case 0x8: out[i] = '8'; break;
                    case 0x9: out[i] = '9'; break;
                    case 0xa: out[i] = 'A'; break;
                    case 0xb: out[i] = 'B'; break;
                    case 0xc: out[i] = 'C'; break;
                    case 0xd: out[i] = 'D'; break;
                    case 0xe: out[i] = 'E'; break;
                    case 0xf: out[i] = 'F'; break;
                }
            }
        }
    }
    
#if USE_CORE_FOR_RF
    static const constexpr int MAX_MSGS = 16;
    static const constexpr int TOTAL_TX_TIME_MS = 100;
    static const constexpr int TIME_BETWEEN_MESSAGES_MS = 25;

    TaskHandle_t s_rfTaskHandle = 0;
    QueueHandle_t s_rfMsgQueue = 0;

    void RFTaskEntry(void*)
    {
        printf("Entering RF\n");
        ShockCollarMsg msg = {};
        ShockCollarMsg txMsg = {};
        int stopTxTime = 0;
        bool transmit = false;
        for(;;)
        {
            bool receivedMsg = xQueueReceive( s_rfMsgQueue, &msg, 10 );
            if ( receivedMsg )
            {
                stopTxTime = millis() + TOTAL_TX_TIME_MS;
                txMsg = msg;
            }
            
            if( millis() < stopTxTime )
            {
                TransmitShockCollarMsg( txMsg );
                delay( TIME_BETWEEN_MESSAGES_MS );
            }
        }
    }
#endif // #if USE_CORE_FOR_RF
}


void ShockCollar::begin()
{    
    ShockCollarSetup();

#if USE_CORE_FOR_RF
    s_rfMsgQueue = xQueueCreate( MAX_MSGS, sizeof( ShockCollarMsg ) );
    if(s_rfTaskHandle == 0)
    {
        xTaskCreate(
        RFTaskEntry, /* Function to implement the task */
        "RFTask", /* Name of the task */
        10000,  /* Stack size in words */
        NULL,  /* Task input parameter */
        tskIDLE_PRIORITY,  /* Priority of the task */
        &s_rfTaskHandle);  /* Task handle. */
    }
#endif // #if USE_CORE_FOR_RF
    redraw_ = true;
    maxPower_ = 0;
    patternIdx_ = 0;
    runPattern_ = false;
    mode_ = TYPE_VIBRATE;
    frameIdx_ = 0;

    lastCommandTime_ = millis();

    resetKeepAwake();
#if USE_CORE_FOR_RF
    ShockCollarMsg msg = BuildShockCollarMsg( RemoteSecret, 0, TYPE_BEEP, 0 );
    xQueueSendToBack( s_rfMsgQueue, &msg, 1000 );
#else // #if USE_CORE_FOR_RF
    TransmitShockCollarMsg( msg );
#endif // #else // #if USE_CORE_FOR_RF

    App::begin();
}

void ShockCollar::end()
{
#if USE_CORE_FOR_RF
    vTaskDelete(s_rfTaskHandle);
    vQueueDelete(s_rfMsgQueue);
    s_rfTaskHandle = 0;
#endif // #if USE_CORE_FOR_RF

    App::end();
}


void ShockCollar::key_event(uint8_t key)
{
    ShockCollarMsg msg = {};

    const int debounceMs = 250;
    const int timeSinceLastInput = micros() - lastInputTime_;
    lastInputTime_ = micros();

    switch( key )
    {
        case APP_KEY_SCROLL_CLOCKWISE:
            {
                if( timeSinceLastInput < debounceMs )
                {
                    if(powerAdjustRate_ < 10)
                        powerAdjustRate_++;
                }
                else
                {
                    powerAdjustRate_ = 1;
                }

                const uint8_t nextPower = (uint8_t)std::min((int)maxPower_ + (int)powerAdjustRate_, 99);
                if( maxPower_ != nextPower )
                {
                    maxPower_ = nextPower;
                    redraw_ = true;
                }
            }
            break;

        case APP_KEY_SCROLL_ANTICLOCKWISE:
            {
                if( timeSinceLastInput < debounceMs )
                {
                    if(powerAdjustRate_ < 10)
                        powerAdjustRate_++;
                }
                else
                {
                    powerAdjustRate_ = 1;
                }

                const int nextPower = (uint8_t)std::max((int)maxPower_ - (int)powerAdjustRate_, 0);
                if( maxPower_ != nextPower )
                {
                    maxPower_ = nextPower;
                    redraw_ = true;
                }
            }
            break;

        case APP_KEY_OK:
            if( timeSinceLastInput > debounceMs )
            {
                if( mode_ == TYPE_VIBRATE )
                {
                    mode_ = TYPE_SHOCK;
                    maxPower_ = 0; // Reset power to zero when switching to shock.
                    powerAdjustRate_ = 1;

                    // Send a beep.
                    msg = BuildShockCollarMsg( RemoteSecret, 0, TYPE_BEEP, 0 );
#if USE_CORE_FOR_RF
                    xQueueSendToBack( s_rfMsgQueue, &msg, 1000 );
#else // #if USE_CORE_FOR_RF
                    TransmitShockCollarMsg( msg );
#endif // #else // #if USE_CORE_FOR_RF
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
            }
            break;

        case APP_KEY_FORWARD:
            if( patternIdx_ < PATTERN_COUNT - 1 )
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
            if( timeSinceLastInput > debounceMs || runPattern_ )
            {
                runPattern_ = !runPattern_;
                frameIdx_ = 0;
                redraw_ = true;

                // Send a beep.
                msg = BuildShockCollarMsg( RemoteSecret, 0, TYPE_BEEP, 0 );
#if USE_CORE_FOR_RF
                xQueueSendToBack( s_rfMsgQueue, &msg, 1000 );
#else // #if USE_CORE_FOR_RF
                TransmitShockCollarMsg( msg );
#endif // #else // #if USE_CORE_FOR_RF
            }
            break;

        case APP_KEY_MENU:
            if( timeSinceLastInput > debounceMs )
            {
                App::manager_end();
            }
            break;
    }

    lastInputTime_ = micros();
}

void ShockCollar::draw(Adafruit_GFX &gfx)
{
    const uint8_t patternTime = runPattern_ ? ( frameIdx_ ) % PATTERN_SIZE : PATTERN_SIZE;
    bool sendCommand = false;

    char buf[100];

    int now = millis();
    int timeSinceLastCommand = now - lastCommandTime_;
    if( timeSinceLastCommand > commandRateMs_ )
    {
        lastCommandTime_ += commandRateMs_;
        sendCommand = runPattern_;
    }

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

        int16_t x = 8;
        int16_t y = 8;

        y += 16;
        gfx.setCursor(x, y);
        gfx.printf("Max Power: %u", maxPower_);

        y += 16;
        gfx.setCursor(x, y);
        gfx.print("Pattern:");

        y += 16;
        memset(buf, 0, sizeof(buf));
        patternString(buf, patternIdx_, patternTime);
        gfx.setCursor(x, y);
        gfx.print(buf);

        y += 32;
        memset(buf, 0, sizeof(buf));
        gfx.setCursor(x, y);
        gfx.printf("Mode: %s", modes[mode_]); 

        y += 16;
        gfx.setCursor(x, y);
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
#if USE_CORE_FOR_RF
                xQueueSendToBack( s_rfMsgQueue, &msg, 0 );
#else // #if USE_CORE_FOR_RF
                TransmitShockCollarMsg( msg );
#endif // #else // #if USE_CORE_FOR_RF
                
                resetKeepAwake();
            }
            frameIdx_++;
        }
    }

    if( now > nextKeepAwakeMsgMs_ )
    {
        resetKeepAwake();
#if USE_CORE_FOR_RF
        ShockCollarMsg msg = BuildShockCollarMsg( RemoteSecret, 0, TYPE_VIBRATE, 1 );
        xQueueSendToBack( s_rfMsgQueue, &msg, 1000 );
#else // #if USE_CORE_FOR_RF
        TransmitShockCollarMsg( msg );
#endif // #else // #if USE_CORE_FOR_RF
    }
}

uint8_t ShockCollar::calculatePower( uint8_t power ) const 
{
    uint16_t actualPower = ((uint16_t)power * (uint16_t)maxPower_ * 6) / 99;
    if(actualPower > maxPower_)
        actualPower = maxPower_;
    return actualPower;
}
