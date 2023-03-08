#include "App.hpp"

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

void App::manager_key_event(uint8_t key) {
    App::app().key_event(key);
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

void App::draw(Adafruit_GFX &gfx) {

}

void App::loop(void) {

}

void App::hidden(void) {

}

void App::end(void) {
    
}
