#pragma once

#include <App/Menu.hpp>

class GameMenu : public Menu  {
    public:
        GameMenu() {};
        virtual ~GameMenu() {};
        virtual void begin(BpodMenu &menu);
};
