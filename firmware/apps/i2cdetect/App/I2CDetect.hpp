#pragma once

#include <stdint.h>
#include <string>

#include <App/App.hpp>

class I2CDetect : public App  {
    public:
        I2CDetect() : addr_(0xff), prev_addr_(0xff), redraw_(true), selectable_(false), scan_addr_(0) {};
        virtual ~I2CDetect() {};

        virtual void begin(void);
        virtual void loop(void);
        virtual void key_event(uint8_t key);
        virtual void draw(Adafruit_GFX &gfx);
        virtual void end(void);

        void set_addr_state(uint8_t addr, bool present);
        bool addr_present(uint8_t addr);
        bool addr_selected(uint8_t addr);
        bool addr_state_changed(uint8_t addr);

        uint8_t addr() const { return addr_; };
        void set_addr(uint8_t addr) { addr_ = addr; };
        void selectable() { selectable_ = true; }

    private:
        uint8_t addr_;
        uint8_t prev_addr_;
        uint8_t scan_addr_;
        bool redraw_;
        bool selectable_;
        uint8_t detected_[128/8];
        uint8_t prev_detected_[128/8];
};
