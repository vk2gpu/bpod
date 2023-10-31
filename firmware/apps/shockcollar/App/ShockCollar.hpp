#pragma once

#include <App/Menu.hpp>

class ShockCollar : public Menu  {
    public:
        ShockCollar() {};
        virtual ~ShockCollar() {};
        virtual void begin();
        virtual void begin(BpodMenu &menu);
};
