#include "GameMenu.hpp"

#include <App/Snake.hpp>
#include <App/Tetris.hpp>
#include <ctf.h>

Snake snake;
Tetris tetris;

void GameMenu::begin(BpodMenu &menu) {
    menu.set_title("Games");
    menu.add("Snake", [](){ App::manager_begin(snake); });
    menu.add("Tetris", [](){ App::manager_begin(tetris); });

    CTF_OFF_MACRO
}

void GameMenu::end()
{
    CTF_ON_MACRO
    Menu::end();
}
