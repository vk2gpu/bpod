#pragma once

#include <App/Menu.hpp>

class MainMenu : public Menu  {
    public:
        MainMenu() {};
        virtual ~MainMenu() {};
        virtual void begin(BpodMenu &menu);
};
