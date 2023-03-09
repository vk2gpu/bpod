#pragma once

#include <App/App.hpp>

#include <snakec.h>

#define SNAKE_GRID_WIDTH        20
#define SNAKE_GRID_HEIGHT       20
#define SNAKE_SQUARE_SIZE       6
#define SNAKE_GRID_OFFSET_X     2
#define SNAKE_GRID_OFFSET_Y     22
#define SNAKE_BOARDER_INDENT    2
#define SNAKE_BOARDER_WIDTH     ((SNAKE_GRID_WIDTH*SNAKE_SQUARE_SIZE)+(2*SNAKE_BOARDER_INDENT))
#define SNAKE_BOARDER_HEIGTH    ((SNAKE_GRID_HEIGHT*SNAKE_SQUARE_SIZE)+(2*SNAKE_BOARDER_INDENT))

class Snake : public App  {
    public:
        Snake() : draw_gfx(nullptr), direction(0) {};
        virtual ~Snake() {};

        virtual void begin(void);
        virtual void key_event(uint8_t key);
        virtual void draw(Adafruit_GFX &gfx);
        virtual void end(void);

        Adafruit_GFX *draw_gfx;

        uint8_t snake_data[SNKC_CALC_DATA_SIZE(SNAKE_GRID_WIDTH, SNAKE_GRID_HEIGHT)];

    private:
        uint8_t direction;
};
