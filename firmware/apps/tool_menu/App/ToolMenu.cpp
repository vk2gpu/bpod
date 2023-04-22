#include "ToolMenu.hpp"

#include <App/I2CDetect.hpp>
#include <App/I2CSniff.hpp>
#include <App/UARTSniff.hpp>
#include <App/Leds.hpp>
#include <App/MCP23017.hpp>
#include <App/MCP23S17.hpp>
#include <ctf.h>

I2CDetect i2cdetect;
I2CSniff i2csniff;
UARTSniff uartsniff;
Leds leds;
MCP23017 mcp23017;
MCP23S17 mcp23S17;

void ToolMenu::begin(BpodMenu &menu)
{
    menu.set_title("Tools");
    menu.add("i2cdetect", [](){ App::manager_begin(i2cdetect); });
    menu.add("i2csniff", [](){ App::manager_begin(i2csniff); });
    menu.add("uartsniff", [](){ App::manager_begin(uartsniff); });
    menu.add("leds", [](){ App::manager_begin(leds); });
    menu.add("mcp23017", [](){ App::manager_begin(mcp23017); });
    menu.add("mcp23S17", [](){ App::manager_begin(mcp23S17); });

    CTF_OFF_MACRO
}

void ToolMenu::end()
{
    CTF_ON_MACRO
    Menu::end();
}
