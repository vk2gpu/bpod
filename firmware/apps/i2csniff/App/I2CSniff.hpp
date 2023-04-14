#pragma once

#include <stdint.h>
#include <string>

#include <App/App.hpp>
#include <App/TextView.hpp>

class I2CSniff : public TextView  {
    public:
        I2CSniff() : pause_(false) {};
        virtual ~I2CSniff() {};

        virtual void begin();
        virtual void loop();
        virtual void key_event(uint8_t key);
        virtual void end();

    private:
        bool pause_;
        uint32_t last_parse_;
};
