#include "MainMenu.hpp"

#include <App/Schedule.hpp>
#include <App/GameMenu.hpp>
#include <App/TextView.hpp>
#include <App/QrCodeView.hpp>
#include <App/ToolMenu.hpp>
#include <App/About.hpp>
#include <score.h>

Schedule scheudle;
GameMenu game_menu;
QrCodeView website;
ToolMenu tools;
About about;

void MainMenu::begin(BpodMenu &menu) {
    menu.set_title("bPod");
    menu.add("Schedule", [](){ App::manager_begin(scheudle); });
    menu.add("Games", [](){ App::manager_begin(game_menu); });
    menu.add("Tools", [](){ App::manager_begin(tools); });
    menu.add("Reboot", [](){ esp_restart(); });
    menu.add("Website", [](){
        website.set_title("BSides");
        website.set_text("https://www.bsidesau.com.au/");
        App::manager_begin(website);
    });
    menu.add("About", [](){ App::manager_begin(about); });
    set_rm((read_mac_t)(void*)esp_efuse_mac_get_default);
}
