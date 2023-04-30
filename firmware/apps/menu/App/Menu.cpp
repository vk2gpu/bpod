#include "Menu.hpp"

void Menu::begin(void) {
    menu_.clear();
    begin(menu_);
}

void Menu::key_event(uint8_t key) {
    switch( key )
    {
        case APP_KEY_SCROLL_CLOCKWISE:
            menu_.move_down();
            break;

        case APP_KEY_SCROLL_ANTICLOCKWISE:
            menu_.move_up();
            break;

        case APP_KEY_OK:
            menu_.click();
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
        menu_.redraw(gfx);
        return;
    }
    menu_.draw(gfx);
}

void Menu::visible() {
    redraw_ = true;
}

void Menu::end(void) {
    menu_.clear();
}
