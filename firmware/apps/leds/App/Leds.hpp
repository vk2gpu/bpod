#pragma once

#include <App/Menu.hpp>

class Leds : public Menu  {
    public:
        Leds() {};
        virtual ~Leds() {};
        virtual void begin();
        virtual void begin(BpodMenu &menu);
};
