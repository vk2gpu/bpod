#pragma once

#include <App/Menu.hpp>

class GPIOs : public Menu  {
    public:
        GPIOs() {};
        virtual ~GPIOs() {};
        virtual void begin();
        virtual void begin(BpodMenu &menu);
};
