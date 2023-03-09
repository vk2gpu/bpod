#include "Tetris.hpp"

void TTRS_API tetrisc_begin_frame(void *ctx)
{
    Tetris &tetris = *reinterpret_cast<Tetris*>(ctx);
    Adafruit_GFX &gfx = *tetris.draw_gfx;
    if ( !tetris.draw_gfx )
    {
        return;
    }
    gfx.fillScreen(0xffff);
    gfx.setTextColor(0x0000);
    gfx.setCursor(8, 0);
    gfx.setTextSize(2, 2);
    char score_text[20];
    sprintf(score_text, "%8d", ttrs_get_score(tetris.tetris_data) * 100);
    gfx.print(score_text);
    gfx.drawRect(TETRIS_GRID_OFFSET_X, TETRIS_GRID_OFFSET_Y, TETRIS_BOARDER_WIDTH, TETRIS_BOARDER_HEIGTH, 0x0000);
}

void TTRS_API tetrisc_end_frame(void *ctx)
{
    Tetris &tetris = *reinterpret_cast<Tetris*>(ctx);
    Adafruit_GFX &gfx = *tetris.draw_gfx;
    if ( !tetris.draw_gfx )
    {
        return;
    }
    gfx.setCursor(TETRIS_NEXT_OFFSET_X + TETRIS_SQUARE_SIZE, TETRIS_NEXT_OFFSET_Y - TETRIS_SQUARE_SIZE);
    gfx.setTextColor(0x0000);
    gfx.setTextSize(1, 1);
    gfx.print("NEXT");
}

void TTRS_API tetrisc_draw_piece(void *ctx, int16_t x,int16_t y, TTRS_PIECE_TYPE type)
{
    Tetris &tetris = *reinterpret_cast<Tetris*>(ctx);
    Adafruit_GFX &gfx = *tetris.draw_gfx;
    if ( !tetris.draw_gfx )
    {
        return;
    }
    x = TETRIS_GRID_OFFSET_X + TETRIS_BOARDER_INDENT + (x * TETRIS_SQUARE_SIZE);
    y = TETRIS_GRID_OFFSET_Y + TETRIS_BOARDER_INDENT + (y * TETRIS_SQUARE_SIZE);
    gfx.drawRect(x, y, TETRIS_SQUARE_SIZE, TETRIS_SQUARE_SIZE, 0x0000);
}

void TTRS_API tetrisc_draw_next_piece(void *ctx, int16_t x,int16_t y, TTRS_PIECE_TYPE type)
{
    Tetris &tetris = *reinterpret_cast<Tetris*>(ctx);
    Adafruit_GFX &gfx = *tetris.draw_gfx;
    if ( !tetris.draw_gfx )
    {
        return;
    }
    x = TETRIS_NEXT_OFFSET_X + (x * TETRIS_SQUARE_SIZE);
    y = TETRIS_NEXT_OFFSET_Y + (y * TETRIS_SQUARE_SIZE);
    gfx.drawRect(x, y, TETRIS_SQUARE_SIZE, TETRIS_SQUARE_SIZE, 0x0000);
}

void TTRS_API tetrisc_draw_block(void *ctx, int16_t x,int16_t y)
{
    Tetris &tetris = *reinterpret_cast<Tetris*>(ctx);
    Adafruit_GFX &gfx = *tetris.draw_gfx;
    if ( !tetris.draw_gfx )
    {
        return;
    }
    x = TETRIS_GRID_OFFSET_X + TETRIS_BOARDER_INDENT + (x * TETRIS_SQUARE_SIZE);
    y = TETRIS_GRID_OFFSET_Y + TETRIS_BOARDER_INDENT + (y * TETRIS_SQUARE_SIZE);
    gfx.fillRect(x, y, TETRIS_SQUARE_SIZE, TETRIS_SQUARE_SIZE, 0x0000);
}

int16_t TTRS_API tetrisc_random(void *ctx, int16_t min, int16_t max)
{
    return random(min, max + 1);
}

void TTRS_API tetrisc_game_over(void *ctx, uint16_t score)
{
    App::manager_end();
}

void Tetris::begin(void)
{
    draw_gfx = nullptr;
    ttrs_init(tetris_data, sizeof(tetris_data));
    ttrs_set_grid(tetris_data, TETRIS_GRID_WIDTH, TETRIS_GRID_HEIGHT);
    ttrs_set_draw_clear(tetris_data, tetrisc_begin_frame, reinterpret_cast<void*>(this));
    ttrs_set_draw_piece(tetris_data, tetrisc_draw_piece, reinterpret_cast<void*>(this));
    ttrs_set_draw_next_piece(tetris_data, tetrisc_draw_next_piece, reinterpret_cast<void*>(this));
    ttrs_set_draw_block(tetris_data, tetrisc_draw_block, reinterpret_cast<void*>(this));
    ttrs_set_random(tetris_data, tetrisc_random, reinterpret_cast<void*>(this));
    ttrs_set_game_over(tetris_data, tetrisc_game_over, reinterpret_cast<void*>(this));
    ttrs_reset(tetris_data);
}

void Tetris::key_event(uint8_t key)
{
    switch( key )
    {
        case APP_KEY_SCROLL_CLOCKWISE:
            ttrs_key_right(tetris_data);
            tetrisc_end_frame(reinterpret_cast<void*>(this));
            break;

        case APP_KEY_SCROLL_ANTICLOCKWISE:
            ttrs_key_left(tetris_data);
            tetrisc_end_frame(reinterpret_cast<void*>(this));
            break;

        case APP_KEY_OK:
            ttrs_key_rotate(tetris_data);
            tetrisc_end_frame(reinterpret_cast<void*>(this));
            break;

        case APP_KEY_PLAY:
            ttrs_key_drop(tetris_data);
            tetrisc_end_frame(reinterpret_cast<void*>(this));
            break;

        case APP_KEY_MENU:
            App::manager_end();
            break;
    }
}

void Tetris::draw(Adafruit_GFX &gfx)
{
    static uint32_t last_tick = 0;
    uint32_t tick = (uint32_t)millis();
    draw_gfx = &gfx;
    if ( (tick - last_tick) > ((uint32_t)700) )
    {
        last_tick = tick;
        ttrs_tick(tetris_data);
        tetrisc_end_frame(reinterpret_cast<void*>(this));
    }
}

void Tetris::hidden(void)
{
    draw_gfx = nullptr;
}

void Tetris::end(void)
{
    ttrs_fini(tetris_data);
    draw_gfx = nullptr;
}
