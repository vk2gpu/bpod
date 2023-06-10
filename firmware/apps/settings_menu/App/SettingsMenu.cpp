#include "SettingsMenu.hpp"

#include <App/Brightness.hpp>
#include <App/About.hpp>
#include <App/QrCodeView.hpp>
#include <stringdb.h>

Brightness brightness;
About about;
QrCodeView update;

void SettingsMenu::begin(BpodMenu &menu) {
    menu.set_title(STRING(STRING_SETTINGS));
    menu.add(STRING(STRING_BRIGHTNESS), [](){ App::manager_begin(brightness); });
    menu.add(STRING(STRING_REBOOT), [](){ esp_restart(); });
    menu.add(STRING(STRING_UPDATE), [](){
        update.set_title(STRING(STRING_UPDATE));
        update.set_text(STRING(UPDATE_URL));
        App::manager_begin(update);
    });
    menu.add(STRING(STRING_ABOUT), [](){ App::manager_begin(about); });
}
