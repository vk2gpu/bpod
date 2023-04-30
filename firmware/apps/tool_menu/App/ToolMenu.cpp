#include "ToolMenu.hpp"

#include <App/I2CDetect.hpp>
#include <App/I2CSniff.hpp>
#include <App/SPISniff.hpp>
#include <App/UARTTerm.hpp>
#include <App/GPIO.hpp>
#include <App/Leds.hpp>
#include <App/MCP23017.hpp>
#include <App/MCP23S17.hpp>
#include <App/WifiScan.hpp>
#include <ctf.h>

I2CDetect i2cdetect;
I2CSniff i2csniff;
SPISniff spisniff;
UARTTerm uartterm;
GPIOs gpio;
Leds leds;
MCP23017 mcp23017;
MCP23S17 mcp23S17;
WifiScan wifiscan;

void ToolMenu::begin(BpodMenu &menu)
{
    menu.set_title("Tools");
    menu.add("i2cdetect", [](){ App::manager_begin(i2cdetect); });
    menu.add("i2csniff", [](){ App::manager_begin(i2csniff); });
    menu.add("spisniff", [](){ App::manager_begin(spisniff); });
    menu.add("uartterm", [](){ App::manager_begin(uartterm); });
    menu.add("gpio", [](){ App::manager_begin(gpio); });
    menu.add("leds", [](){ App::manager_begin(leds); });
    menu.add("mcp23017", [](){ App::manager_begin(mcp23017); });
    menu.add("mcp23S17", [](){ App::manager_begin(mcp23S17); });
    menu.add("wifiscan", [](){ App::manager_begin(wifiscan); });

    CTF_OFF_MACRO
}

void ToolMenu::end()
{
    CTF_ON_MACRO
    Menu::end();
}
