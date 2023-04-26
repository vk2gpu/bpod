#include "Tetris.hpp"

#include <Images/BpodTitleBar.hpp>
#include <score.h>

static void TTRS_API tetrisc_begin_frame(void *ctx)
{
    reinterpret_cast<Tetris*>(ctx)->draw_begin_frame();
}

static void TTRS_API tetrisc_draw_piece(void *ctx, int16_t x,int16_t y, TTRS_PIECE_TYPE type)
{
    reinterpret_cast<Tetris*>(ctx)->draw_piece(x, y, type);
}

static void TTRS_API tetrisc_draw_next_piece(void *ctx, int16_t x,int16_t y, TTRS_PIECE_TYPE type)
{
    reinterpret_cast<Tetris*>(ctx)->draw_next_piece(x, y, type);
}

static void TTRS_API tetrisc_draw_block(void *ctx, int16_t x,int16_t y)
{
    reinterpret_cast<Tetris*>(ctx)->draw_block(x, y);
}

static int16_t TTRS_API tetrisc_random(void *ctx, int16_t min, int16_t max)
{
    return random(min, max + 1);
}

static void TTRS_API tetrisc_game_over(void *ctx, uint16_t score)
{
    delay(1000);
    App::manager_end();
    char buf[512];
    std::string s(get_surl(buf, sizeof(buf), "http://127.0.0.1:8000/index.html", SCORE_CODE_TETRIS, score));
    reinterpret_cast<Tetris*>(ctx)->s_.set_text(s);
    App::manager_begin(reinterpret_cast<Tetris*>(ctx)->s_);
}

void Tetris::begin(void)
{
    tetris_canvas_ = new uint8_t[TETRIS_GRID_WIDTH * TETRIS_GRID_HEIGHT];
    memset(tetris_canvas_, 0, TETRIS_GRID_WIDTH * TETRIS_GRID_HEIGHT);
    tetris_next_canvas_ = new uint8_t[5 * 5];
    memset(tetris_next_canvas_, 0, 5 * 5);
    ttrs_init(tetris_data, sizeof(tetris_data));
    ttrs_set_grid(tetris_data, TETRIS_GRID_WIDTH, TETRIS_GRID_HEIGHT);
    ttrs_set_draw_clear(tetris_data, tetrisc_begin_frame, reinterpret_cast<void*>(this));
    ttrs_set_draw_piece(tetris_data, tetrisc_draw_piece, reinterpret_cast<void*>(this));
    ttrs_set_draw_next_piece(tetris_data, tetrisc_draw_next_piece, reinterpret_cast<void*>(this));
    ttrs_set_draw_block(tetris_data, tetrisc_draw_block, reinterpret_cast<void*>(this));
    ttrs_set_random(tetris_data, tetrisc_random, reinterpret_cast<void*>(this));
    ttrs_set_game_over(tetris_data, tetrisc_game_over, reinterpret_cast<void*>(this));
    ttrs_reset(tetris_data);
    redraw_ = true;
    draw_end_frame_ = false;
    prev_score_ = 0xffffffff;  // make it redraw on first frame
    piece_type_ = TTRS_PIECE_TYPE_UNKNOWN;
    s_.set_title("Game Over");
}

void Tetris::key_event(uint8_t key)
{
    switch( key )
    {
        case APP_KEY_SCROLL_CLOCKWISE:
            if ( !draw_end_frame_ )
            {
                ttrs_key_right(tetris_data);
            }
            draw_end_frame_ = true;
            break;

        case APP_KEY_SCROLL_ANTICLOCKWISE:
        if ( !draw_end_frame_ )
            {
                ttrs_key_left(tetris_data);
            }
            draw_end_frame_ = true;
            break;

        case APP_KEY_OK:
        if ( !draw_end_frame_ )
            {
                ttrs_key_rotate(tetris_data);
            }
            draw_end_frame_ = true;
            break;

        case APP_KEY_PLAY:
        if ( !draw_end_frame_ )
            {
                ttrs_key_drop(tetris_data);
            }
            draw_end_frame_ = true;
            break;

        case APP_KEY_MENU:
            App::manager_end();
            break;
    }
}

void Tetris::draw_begin_frame()
{
    if ( !tetris_canvas_ || !tetris_next_canvas_ ) return;
    for ( size_t i = 0; i < (TETRIS_GRID_WIDTH * TETRIS_GRID_HEIGHT); i++ )
    {
        // move new nibble into old nibble and clear new nibble
        tetris_canvas_[i] = (tetris_canvas_[i] << 4) & 0xf0;
    }
    for ( size_t i = 0; i < (5 * 5); i++ )
    {
        // move new nibble into old nibble and clear new nibble
        tetris_next_canvas_[i] = (tetris_next_canvas_[i] << 4) & 0xf0;
    }
}

void Tetris::draw_piece(int16_t x,int16_t y, TTRS_PIECE_TYPE type)
{
    if ( !tetris_canvas_ || !tetris_next_canvas_ ) return;
    size_t i = ((size_t)y * TETRIS_GRID_WIDTH) + (size_t)x;
    tetris_canvas_[i] = (tetris_canvas_[i] & 0xf0) | (type + 1);
    piece_type_ = type;
}

void Tetris::draw_next_piece(int16_t x,int16_t y, TTRS_PIECE_TYPE type)
{
    if ( !tetris_canvas_ || !tetris_next_canvas_ ) return;
    size_t i = ((size_t)y * 5) + (size_t)x;
    tetris_next_canvas_[i] = (tetris_next_canvas_[i] & 0xf0) | (type + 1);
}

void Tetris::draw_block(int16_t x, int16_t y)
{
    if ( !tetris_canvas_ || !tetris_next_canvas_ ) return;
    size_t i = ((size_t)y * TETRIS_GRID_WIDTH) + (size_t)x;
    TTRS_PIECE_TYPE type = (TTRS_PIECE_TYPE)(((tetris_canvas_[i] >> 4) - 1) & 0x7);
    if ( TTRS_PIECE_TYPE_UNKNOWN == type )
    {
        type = piece_type_;
    }
    tetris_canvas_[i] = (tetris_canvas_[i] & 0xf0) | 0x8 | (type + 1);
}

void Tetris::draw_block(Adafruit_GFX &gfx, int16_t x, int16_t y, uint8_t block_type)
{
    uint16_t colour = 0x0000;
    TTRS_PIECE_TYPE piece = (TTRS_PIECE_TYPE)((block_type - 1) & 0x7);
    switch ( piece )
    {
        case TTRS_PIECE_TYPE_SQUARE:
            colour = 0xE720;    // yellow
            break;

        case TTRS_PIECE_TYPE_I:
            colour = 0x06D9;    // cyan
            break;

        case TTRS_PIECE_TYPE_L:
            colour = 0xFC00;    // orange
            break;

        case TTRS_PIECE_TYPE_L_MIRRORED:
            colour = 0x001F;    // blue
            break;

        case TTRS_PIECE_TYPE_N:
            colour = 0xF800;    // red
            break;

        case TTRS_PIECE_TYPE_N_MIRRORED:
            colour = 0x0580;    // green
            break;

        case TTRS_PIECE_TYPE_T:
            colour = 0xF81F;    // purple
            break;
    }

    if ( block_type & 0x8 )
    {
        // fixed
        gfx.fillRect(x, y, TETRIS_SQUARE_SIZE, TETRIS_SQUARE_SIZE, colour);
    }
    else if ( block_type & 0x7 )
    {
        // dropping
        gfx.fillRect(x, y, TETRIS_SQUARE_SIZE, TETRIS_SQUARE_SIZE, 0xffff);
        gfx.drawRect(x, y, TETRIS_SQUARE_SIZE, TETRIS_SQUARE_SIZE, colour);
    }
    else
    {
        // empty
        gfx.fillRect(x, y, TETRIS_SQUARE_SIZE, TETRIS_SQUARE_SIZE, 0xffff);
    }
}

void Tetris::draw_end_frame(Adafruit_GFX &gfx)
{
    size_t i = 0;
    if ( !tetris_canvas_ || !tetris_next_canvas_ ) return;
    for ( int16_t gy = 0; gy < TETRIS_GRID_HEIGHT; gy++ )
    {
        for ( int16_t gx = 0; gx < TETRIS_GRID_WIDTH; gx++ )
        {
            // check for a change on this spot
            if ( (tetris_canvas_[i] & 0x0f) != (tetris_canvas_[i] >> 4) & 0x0f )
            {
                int16_t x = TETRIS_GRID_OFFSET_X + TETRIS_BOARDER_INDENT + (gx * TETRIS_SQUARE_SIZE);
                int16_t y = TETRIS_GRID_OFFSET_Y + TETRIS_BOARDER_INDENT + (gy * TETRIS_SQUARE_SIZE);
                draw_block(gfx, x, y, tetris_canvas_[i] & 0x0f);
            }

            // next square on canvas
            i++;
        }
    }
    i = 0;
    for ( int16_t gy = 0; gy < 5; gy++ )
    {
        for ( int16_t gx = 0; gx < 5; gx++ )
        {
            // check for a change on this spot
            if ( (tetris_next_canvas_[i] & 0x0f) != (tetris_next_canvas_[i] >> 4) & 0x0f )
            {
                int16_t x = TETRIS_NEXT_OFFSET_X + (gx * TETRIS_SQUARE_SIZE);
                int16_t y = TETRIS_NEXT_OFFSET_Y + (gy * TETRIS_SQUARE_SIZE);
                draw_block(gfx, x, y, tetris_next_canvas_[i] & 0x0f);
            }

            // next square on canvas
            i++;
        }
    }
}

void Tetris::draw(Adafruit_GFX &gfx)
{
    // redraw on game start
    if ( redraw_ )
    {
        BpodTitleBar::draw(gfx, "Tetris");
        gfx.fillRect(0, BpodTitleBar::view_y(gfx), gfx.width(), BpodTitleBar::view_height(gfx), 0xffff);
        gfx.drawRect(TETRIS_GRID_OFFSET_X, TETRIS_GRID_OFFSET_Y, TETRIS_BOARDER_WIDTH, TETRIS_BOARDER_HEIGTH, 0x001F);
        gfx.setCursor(TETRIS_NEXT_OFFSET_X + TETRIS_SQUARE_SIZE, TETRIS_NEXT_OFFSET_Y - TETRIS_SQUARE_SIZE - 2);
        gfx.setTextColor(0x0000);
        gfx.setTextSize(1, 1);
        gfx.print("NEXT");
        redraw_ = false;
    }

    // key press, end that frame
    if ( draw_end_frame_ )
    {
        draw_end_frame(gfx);
        draw_end_frame_ = false;
    }

    // do stuff (redraw grid if needed)
    static uint32_t last_tick = 0;
    uint32_t tick = (uint32_t)millis();
    if ( (tick - last_tick) > ((uint32_t)700) )
    {
        last_tick = tick;
        ttrs_tick(tetris_data);
        draw_end_frame(gfx);
    }

    // redraw score if its changed
    uint32_t score = ttrs_get_score(this->tetris_data) * 100;
    if ( prev_score_ != score )
    {
        gfx.setTextColor(0x0000, 0xffff);
        int16_t text_height = 8;
        int16_t text_width = 8 * 6;
        gfx.setCursor(gfx.width() - text_width - 1, gfx.height() - text_height - 1);
        gfx.setTextSize(1, 1);
        char score_text[20];
        sprintf(score_text, "%8d", score);
        gfx.print(score_text);
        prev_score_ = score;
    }
}

void Tetris::end(void)
{
    ttrs_fini(tetris_data);
    delete tetris_canvas_;
    tetris_canvas_ = nullptr;
    delete tetris_next_canvas_;
    tetris_next_canvas_ = nullptr;
}
