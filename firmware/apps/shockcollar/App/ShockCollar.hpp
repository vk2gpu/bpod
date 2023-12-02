#pragma once

#include <App/Menu.hpp>

class ShockCollar : public App  {
    public:
        ShockCollar() {};
        virtual ~ShockCollar() {};
        virtual void begin();
        virtual void end(void);

        virtual void key_event(uint8_t key);
        virtual void draw(Adafruit_GFX &gfx);

    private:
        uint8_t calculatePower( uint8_t power ) const;
        void resetKeepAwake()
        {
            nextKeepAwakeMsgMs_ = millis() + 30000;
        }

        bool redraw_ = true;
        uint8_t maxPower_ = 0;
        uint8_t patternIdx_ = 0;
        bool runPattern_ = false;
        uint8_t mode_ = 0;
        uint16_t frameIdx_ = 0;
        int lastCommandTime_ = 0;
        int commandRateMs_ = 300;
        int nextKeepAwakeMsgMs_ = 0;
        int lastInputTime_ = 0;
        uint8_t powerAdjustRate_ = 1;
};
