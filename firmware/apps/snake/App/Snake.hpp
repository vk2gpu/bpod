#pragma once

#include <App/App.hpp>

#include <snakec.h>

#define SNAKE_GRID_WIDTH        20
#define SNAKE_GRID_HEIGHT       20
#define SNAKE_SQUARE_SIZE       6
#define SNAKE_GRID_OFFSET_X     2
#define SNAKE_GRID_OFFSET_Y     23
#define SNAKE_BOARDER_INDENT    2
#define SNAKE_BOARDER_WIDTH     ((SNAKE_GRID_WIDTH*SNAKE_SQUARE_SIZE)+(2*SNAKE_BOARDER_INDENT))
#define SNAKE_BOARDER_HEIGTH    ((SNAKE_GRID_HEIGHT*SNAKE_SQUARE_SIZE)+(2*SNAKE_BOARDER_INDENT))

class Snake : public App  {
    public:
        Snake() : snake_canvas_(nullptr), direction_(0), redraw_(true), prev_score_(0), wait_for_draw_(false) {};
        virtual ~Snake() {};

        virtual void begin(void);
        virtual void key_event(uint8_t key);
        virtual void draw(Adafruit_GFX &gfx);
        virtual void end(void);

        void draw_begin_frame();
        void draw_snake(int16_t x, int16_t y);
        void draw_apple(int16_t x, int16_t y);
        void draw_end_frame(Adafruit_GFX &gfx);

    private:
        uint8_t *snake_canvas_;
        uint8_t direction_;
        bool redraw_;
        uint32_t prev_score_;
        bool wait_for_draw_;
        uint8_t snake_data_[SNKC_CALC_DATA_SIZE(SNAKE_GRID_WIDTH, SNAKE_GRID_HEIGHT)];
};
