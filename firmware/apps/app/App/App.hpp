#pragma once

#include <vector>
#include <Adafruit_GFX.h>

enum {
    APP_KEY_NONE,
    APP_KEY_SCROLL_CLOCKWISE,
    APP_KEY_SCROLL_ANTICLOCKWISE,
    APP_KEY_FORWARD,
    APP_KEY_MENU,
    APP_KEY_BACK,
    APP_KEY_PLAY,
    APP_KEY_OK,
};

class App  {
    public:
        App() {};
        virtual ~App() {};

        static void manager_begin(App &app);
        static void manager_key_event(uint8_t key);
        static void manager_keyboard_event(int key);
        static void manager_draw(Adafruit_GFX &gfx);
        static void manager_loop(void);
        static void manager_end();

        virtual void begin(void);
        virtual void visible();
        virtual void key_event(uint8_t key);
        virtual void keyboard_event(int key);
        virtual void draw(Adafruit_GFX &gfx);
        virtual void loop(void);
        virtual void hidden(void);
        virtual void end(void);
    
    private:
        static App &app(void);
        static App app_null;
        static std::vector<App*> app_stack;
};
