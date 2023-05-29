#pragma once

#include <stdint.h>
#include <string>

#include <App/App.hpp>

class Brightness : public App  {
    public:
        Brightness() : redraw_(true), brightness_(0) {};
        virtual ~Brightness() {};

        static void setup(int pin);
        static uint8_t read();
        static void write(uint8_t percent);

        virtual void begin(void);
        virtual void key_event(uint8_t key);
        virtual void draw(Adafruit_GFX &gfx);

    private:
        bool redraw_;
        uint8_t brightness_;
};
