#include "ToolMenu.hpp"

#include <App/I2CDetect.hpp>

I2CDetect i2cdetect;

void ToolMenu::begin(BpodMenu &menu) {
    menu.set_title("Tools");
    menu.add("i2cdetect", [](){ App::manager_begin(i2cdetect); });
}
