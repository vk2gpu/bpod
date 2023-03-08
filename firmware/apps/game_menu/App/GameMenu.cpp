#include "GameMenu.hpp"

void GameMenu::begin(BpodMenu &menu) {
    menu.set_title("Games");
    menu.add("Snake", [](){});
    menu.add("Tetris", [](){});
}
