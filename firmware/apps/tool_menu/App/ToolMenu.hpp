#pragma once

#include <App/Menu.hpp>

class ToolMenu : public Menu  {
    public:
        ToolMenu() {};
        virtual ~ToolMenu() {};
        virtual void begin(BpodMenu &menu);
};
