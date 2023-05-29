#pragma once

#include <App/Menu.hpp>

class SettingsMenu : public Menu  {
    public:
        SettingsMenu() {};
        virtual ~SettingsMenu() {};
        virtual void begin(BpodMenu &menu);
};
