#include "MainMenu.hpp"

#include <App/MusicMenu.hpp>
#include <App/Schedule.hpp>
#include <App/GameMenu.hpp>
#include <App/TextView.hpp>
#include <App/QrCodeView.hpp>
#include <App/ToolMenu.hpp>
#include <App/SettingsMenu.hpp>
#include <score.h>
#include <stringdb.h>

MusicMenu music_menu;
Schedule scheudle;
GameMenu game_menu;
QrCodeView website;
ToolMenu tools_menu;
SettingsMenu settings_menu;

void MainMenu::begin(BpodMenu &menu) {
    menu.set_title(STRING(STRING_BPOD));
    menu.add(STRING(STRING_MUSIC), [](){ App::manager_begin(music_menu); });
    menu.add(STRING(STRING_SCHEDULE), [](){ App::manager_begin(scheudle); });
    menu.add(STRING(STRING_GAMES), [](){ App::manager_begin(game_menu); });
    menu.add(STRING(STRING_TOOLS), [](){ App::manager_begin(tools_menu); });
    menu.add(STRING(STRING_WEBSITE), [](){
        website.set_title(STRING(STRING_BSIDES));
        website.set_text(STRING(URL_BSIDES));
        App::manager_begin(website);
    });
    menu.add(STRING(STRING_SETTINGS), [](){ App::manager_begin(settings_menu); });
    set_rm((read_mac_t)(void*)esp_efuse_mac_get_default);
}
