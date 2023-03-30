#include "MainMenu.hpp"

#include <App/Schedule.hpp>
#include <App/GameMenu.hpp>
#include <App/TextView.hpp>
#include <App/QrCodeView.hpp>

Schedule scheudle;
GameMenu game_menu;
TextView about;
QrCodeView website;

void MainMenu::begin(BpodMenu &menu) {
    menu.set_title("bPod");
    menu.add("Schedule", [](){ App::manager_begin(scheudle); });
    menu.add("Games", [](){ App::manager_begin(game_menu); });
    menu.add("Reboot", [](){ esp_restart(); });
    menu.add("Website", [](){
        website.set_title("BSides");
        website.set_text("https://www.bsidesau.com.au/");
        App::manager_begin(website);
    });
    menu.add("About", [](){
        about.set_title("About");
        about.set_text("Made by Peter\n@rankstar591");
        App::manager_begin(about);
    });
}
