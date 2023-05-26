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
#include <stringdb.h>

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
    menu.set_title(STRING(STRING_TOOLS));
    menu.add(STRING(STRING_I2CDETECT), [](){ App::manager_begin(i2cdetect); });
    menu.add(STRING(STRING_I2CSNIFF), [](){ App::manager_begin(i2csniff); });
    menu.add(STRING(STRING_SPISNIFF), [](){ App::manager_begin(spisniff); });
    menu.add(STRING(STRING_UARTTERM), [](){ App::manager_begin(uartterm); });
    menu.add(STRING(STRING_GPIO_LOWER), [](){ App::manager_begin(gpio); });
    menu.add(STRING(STRING_LEDS_LOWER), [](){ App::manager_begin(leds); });
    menu.add(STRING(STRING_MCP23017_LOWER), [](){ App::manager_begin(mcp23017); });
    menu.add(STRING(STRING_MCP23S17_LOWER), [](){ App::manager_begin(mcp23S17); });
    menu.add(STRING(STRING_WIFISCAN), [](){ App::manager_begin(wifiscan); });

    CTF_OFF_MACRO
}

void ToolMenu::end()
{
    CTF_ON_MACRO
    Menu::end();
}
