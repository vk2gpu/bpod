#include "SettingsMenu.hpp"

#include <App/Brightness.hpp>
#include <App/About.hpp>
#include <stringdb.h>

Brightness brightness;
About about;

void SettingsMenu::begin(BpodMenu &menu) {
    menu.set_title(STRING(STRING_SETTINGS));
    menu.add(STRING(STRING_BRIGHTNESS), [](){ App::manager_begin(brightness); });
    menu.add(STRING(STRING_REBOOT), [](){ esp_restart(); });
    menu.add(STRING(STRING_ABOUT), [](){ App::manager_begin(about); });
}
