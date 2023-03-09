#pragma once

#include <App/App.hpp>

#include <tetrisc.h>

#define TETRIS_GRID_WIDTH        10
#define TETRIS_GRID_HEIGHT       20
#define TETRIS_SQUARE_SIZE       6
#define TETRIS_GRID_OFFSET_X     34
#define TETRIS_GRID_OFFSET_Y     20
#define TETRIS_NEXT_OFFSET_X     0
#define TETRIS_NEXT_OFFSET_Y     60
#define TETRIS_BOARDER_INDENT    2
#define TETRIS_BOARDER_WIDTH     ((TETRIS_GRID_WIDTH*TETRIS_SQUARE_SIZE)+(2*TETRIS_BOARDER_INDENT))
#define TETRIS_BOARDER_HEIGTH    ((TETRIS_GRID_HEIGHT*TETRIS_SQUARE_SIZE)+(2*TETRIS_BOARDER_INDENT))

class Tetris : public App  {
    public:
        Tetris() {};
        virtual ~Tetris() {};

        virtual void begin(void);
        virtual void key_event(uint8_t key);
        virtual void draw(Adafruit_GFX &gfx);
        virtual void hidden(void);
        virtual void end(void);

        Adafruit_GFX *draw_gfx;

        uint8_t tetris_data[TTRS_CALC_DATA_SIZE(TETRIS_GRID_WIDTH, TETRIS_GRID_HEIGHT)];
};
