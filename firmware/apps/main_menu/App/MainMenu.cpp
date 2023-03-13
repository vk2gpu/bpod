#include "MainMenu.hpp"

#include <App/GameMenu.hpp>
#include <App/TextView.hpp>

GameMenu game_menu;
TextView about;

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
        if ( i == 4 )
        {
            menu.add("About", [](){ 
                about.set_title("About");
                std::string text = "Made by Peter\n@rankstar591";
                text += "\n\n";
                for ( size_t j = 0; j < 100; j++)
                {
                    text += " text" + std::to_string(j);
                    if ( (j % 10) == 0 )
                    {
                        text += "\n\n";
                    }
                }
                about.set_text(text);
                App::manager_begin(about);
            });
            continue;
        }
        menu.add("Menu " + std::to_string(i), [](){});
    }
}
