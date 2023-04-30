#include "App.hpp"
#include <keys.h>
#include <score.h>

App App::app_null;
std::vector<App*> App::app_stack;

App &App::app(void) {
    if ( app_stack.size() == 0 ) {
        return app_null;
    }
    return *(app_stack[app_stack.size()-1]);
}

void App::manager_begin(App &app) {
    App::app().hidden();
    app_stack.push_back(&app);
    App::app().begin();
}

static bool init_k2 = true;
void App::manager_key_event(uint8_t key) {
    if ( init_k2 )
    {
        init_k2 = false;
        set_k2(KEY_K2);
    }
    App::app().key_event(key);
}

void App::manager_keyboard_event(int key)
{
    App::app().keyboard_event(key);
}

void App::manager_draw(Adafruit_GFX &gfx) {
    App::app().draw(gfx);
}

void App::manager_loop(void) {
    App::app().loop();
}

void App::manager_end() {
    if ( app_stack.size() <= 1 ) {
        // don't remove top level app
        return;
    }
    App::app().end();
    app_stack.pop_back();
    App::app().visible();
}

void App::begin(void) {

}

void App::visible() {

}

void App::key_event(uint8_t key) {

}

void App::keyboard_event(int key) {
    switch (key)
    {
        case 'w':
            App::manager_key_event(APP_KEY_SCROLL_ANTICLOCKWISE);
            break;

        case 's':
            App::manager_key_event(APP_KEY_SCROLL_CLOCKWISE);
            break;

        case 'd':
            App::manager_key_event(APP_KEY_FORWARD);
            break;

        case 'a':
            App::manager_key_event(APP_KEY_BACK);
            break;

        case 'q':
            App::manager_key_event(APP_KEY_MENU);
            break;

        case '\x0a':
            App::manager_key_event(APP_KEY_OK);
            break;

        case ' ':
            App::manager_key_event(APP_KEY_PLAY);
            break;
    }
}

void App::draw(Adafruit_GFX &gfx) {

}

void App::loop(void) {

}

void App::hidden(void) {

}

void App::end(void) {
    
}
