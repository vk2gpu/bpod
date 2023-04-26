#pragma once

#include <App/App.hpp>
#include <App/QrCodeView.hpp>

#include <tetrisc.h>

#define TETRIS_GRID_WIDTH        10
#define TETRIS_GRID_HEIGHT       20
#define TETRIS_SQUARE_SIZE       6
#define TETRIS_GRID_OFFSET_X     34
#define TETRIS_GRID_OFFSET_Y     23
#define TETRIS_NEXT_OFFSET_X     1
#define TETRIS_NEXT_OFFSET_Y     72
#define TETRIS_BOARDER_INDENT    2
#define TETRIS_BOARDER_WIDTH     ((TETRIS_GRID_WIDTH*TETRIS_SQUARE_SIZE)+(2*TETRIS_BOARDER_INDENT))
#define TETRIS_BOARDER_HEIGTH    ((TETRIS_GRID_HEIGHT*TETRIS_SQUARE_SIZE)+(2*TETRIS_BOARDER_INDENT))

class Tetris : public App  {
    public:
        Tetris() : tetris_canvas_(nullptr), tetris_next_canvas_(nullptr),
            redraw_(true), draw_end_frame_(false), prev_score_(0), piece_type_(TTRS_PIECE_TYPE_UNKNOWN) {};
        virtual ~Tetris() {};

        virtual void begin(void);
        virtual void key_event(uint8_t key);
        virtual void draw(Adafruit_GFX &gfx);
        virtual void end(void);

        void draw_begin_frame();
        void draw_piece(int16_t x,int16_t y, TTRS_PIECE_TYPE type);
        void draw_next_piece(int16_t x,int16_t y, TTRS_PIECE_TYPE type);
        void draw_block(int16_t x, int16_t y);
        void draw_block(Adafruit_GFX &gfx, int16_t x, int16_t y, uint8_t block_type);
        void draw_end_frame(Adafruit_GFX &gfx);

    public:
        QrCodeView s_;
    private:
        uint8_t *tetris_canvas_;
        uint8_t *tetris_next_canvas_;
        bool redraw_;
        bool draw_end_frame_;
        uint32_t prev_score_;
        TTRS_PIECE_TYPE piece_type_;
        uint8_t tetris_data[TTRS_CALC_DATA_SIZE(TETRIS_GRID_WIDTH, TETRIS_GRID_HEIGHT)];
};
