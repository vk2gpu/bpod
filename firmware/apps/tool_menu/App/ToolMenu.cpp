#include "ToolMenu.hpp"

#include <App/I2CDetect.hpp>
#include <App/I2CSniff.hpp>
#include <App/SerialMonitor.hpp>
#include <App/MCP23017.hpp>
#include <App/MCP23S17.hpp>

I2CDetect i2cdetect;
I2CSniff i2csniff;
SerialMonitor serial_monitor;
MCP23017 mcp23017;
MCP23S17 mcp23S17;

void ToolMenu::begin(BpodMenu &menu) {
    menu.set_title("Tools");
    menu.add("i2cdetect", [](){ App::manager_begin(i2cdetect); });
    menu.add("i2csniff", [](){ App::manager_begin(i2csniff); });
    menu.add("uartsniff", [](){ App::manager_begin(serial_monitor); });
    menu.add("mcp23017", [](){ App::manager_begin(mcp23017); });
    menu.add("mcp23S17", [](){ App::manager_begin(mcp23S17); });
}
