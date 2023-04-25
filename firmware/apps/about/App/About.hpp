#pragma once

#include <stdint.h>
#include <string>

#include <App/App.hpp>

class About : public App  {
    public:
        About() : redraw_(true) {};
        virtual ~About() {};

        virtual void begin(void);
        virtual void key_event(uint8_t key);
        virtual void draw(Adafruit_GFX &gfx);
        virtual void end(void);

    private:
        bool redraw_;
};
