#pragma once

#include <stdint.h>
#include <string>

#include <App/App.hpp>

class I2CDetect : public App  {
    public:
        I2CDetect() : redraw_(true), scan_addr_(0) {};
        virtual ~I2CDetect() {};

        virtual void begin(void);
        virtual void loop(void);
        virtual void key_event(uint8_t key);
        virtual void draw(Adafruit_GFX &gfx);
        virtual void end(void);

        void set_addr_state(uint8_t addr, bool present);
        bool addr_present(uint8_t addr);
        bool addr_state_changed(uint8_t addr);

    private:
        uint8_t scan_addr_;
        bool redraw_;
        uint8_t detected_[128/8];
        uint8_t prev_detected_[128/8];
};
