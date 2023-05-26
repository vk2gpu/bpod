#include "MainMenu.hpp"

#include <App/Schedule.hpp>
#include <App/GameMenu.hpp>
#include <App/TextView.hpp>
#include <App/QrCodeView.hpp>
#include <App/ToolMenu.hpp>
#include <App/About.hpp>
#include <score.h>
#include <stringdb.h>

Schedule scheudle;
GameMenu game_menu;
QrCodeView website;
ToolMenu tools;
About about;

void MainMenu::begin(BpodMenu &menu) {
    menu.set_title(STRING(STRING_BPOD));
    menu.add(STRING(STRING_SCHEDULE), [](){ App::manager_begin(scheudle); });
    menu.add(STRING(STRING_GAMES), [](){ App::manager_begin(game_menu); });
    menu.add(STRING(STRING_TOOLS), [](){ App::manager_begin(tools); });
    menu.add(STRING(STRING_REBOOT), [](){ esp_restart(); });
    menu.add(STRING(STRING_WEBSITE), [](){
        website.set_title(STRING(STRING_BSIDES));
        website.set_text(STRING(URL_BSIDES));
        App::manager_begin(website);
    });
    menu.add(STRING(STRING_ABOUT), [](){ App::manager_begin(about); });
    set_rm((read_mac_t)(void*)esp_efuse_mac_get_default);
}
