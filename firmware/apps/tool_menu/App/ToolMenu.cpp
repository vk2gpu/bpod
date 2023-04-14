#include "ToolMenu.hpp"

#include <App/I2CDetect.hpp>
#include <App/SerialMonitor.hpp>

I2CDetect i2cdetect;
SerialMonitor serial_monitor;

void ToolMenu::begin(BpodMenu &menu) {
    menu.set_title("Tools");
    menu.add("i2cdetect", [](){ App::manager_begin(i2cdetect); });
    menu.add("uartsniff", [](){ App::manager_begin(serial_monitor); });
}
