#include "MainMenu.hpp"

#include <App/GameMenu.hpp>

GameMenu game_menu;

void MainMenu::begin(BpodMenu &menu) {
    menu.set_title("bPod");
    for ( size_t i = 0; i < 50; i++ )
    {
        if ( i == 2 )
        {
            menu.add("Games", [](){ App::manager_begin(game_menu); });
            continue;
        }
        if ( i == 3 )
        {
            menu.add("Reboot", [](){ esp_restart(); });
            continue;
        }
        menu.add("Menu " + std::to_string(i), [](){});
    }
}
