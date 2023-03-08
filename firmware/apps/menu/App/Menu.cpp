#include "Menu.hpp"

void Menu::begin(void) {
    menu.clear();
    begin(menu);
}

void Menu::key_event(uint8_t key) {
    switch( key )
    {
        case APP_KEY_SCROLL_CLOCKWISE:
            menu.move_down();
            break;

        case APP_KEY_SCROLL_ANTICLOCKWISE:
            menu.move_up();
            break;

        case APP_KEY_OK:
            menu.click();
            break;

        case APP_KEY_MENU:
            App::manager_end();
            break;
    }
}

void Menu::draw(Adafruit_GFX &gfx) {
    if ( redraw_ )
    {
        redraw_ = false;
        menu.redraw(gfx);
        return;
    }
    menu.draw(gfx);
}

void Menu::visible() {
    redraw_ = true;
}

void Menu::end(void) {
    menu.clear();
}
