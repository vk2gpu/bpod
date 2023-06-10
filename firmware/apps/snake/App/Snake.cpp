#include "Snake.hpp"

#include <Images/BpodTitleBar.hpp>
#include <Images/SnakeApple6x6.hpp>
#include <score.h>
#include <stringdb.h>

#define SNAKE_KEY_RIGHT     (0)
#define SNAKE_KEY_UP        (1)
#define SNAKE_KEY_LEFT      (2)
#define SNAKE_KEY_DOWN      (3)

void SNKC_API snakec_begin_frame(void *ctx)
{
    reinterpret_cast<Snake*>(ctx)->draw_begin_frame();
}

void SNKC_API snakec_draw_snake(void *ctx, int16_t x,int16_t y)
{
    reinterpret_cast<Snake*>(ctx)->draw_snake(x, y);
}

void SNKC_API snakec_draw_apple(void *ctx, int16_t x,int16_t y)
{
    reinterpret_cast<Snake*>(ctx)->draw_apple(x, y);
}

int16_t SNKC_API snakec_random(void *ctx, int16_t min, int16_t max)
{
    return random(min, max + 1);
}

static void SNKC_API snakec_game_over(void *ctx, uint16_t score)
{
    delay(1000);
    App::manager_end();
    char buf[512];
    char url[512];
    STRING_STRCPY(url, GAME_SCORE_SUBMIT_URL);
    std::string s(get_surl(buf, sizeof(buf), url, SCORE_CODE_SNAKE, score * 0x80));
    reinterpret_cast<Snake*>(ctx)->s_.set_text(s);
    App::manager_begin(reinterpret_cast<Snake*>(ctx)->s_);
}

void Snake::begin(void)
{
    snake_canvas_ = new uint8_t[SNAKE_GRID_WIDTH * SNAKE_GRID_HEIGHT];
    memset(snake_canvas_, 0, SNAKE_GRID_WIDTH * SNAKE_GRID_HEIGHT);
    direction_ = SNAKE_KEY_UP;
    snkc_init(snake_data_, sizeof(snake_data_));
    snkc_set_grid(snake_data_, SNAKE_GRID_WIDTH, SNAKE_GRID_HEIGHT);
    snkc_set_draw_clear(snake_data_, snakec_begin_frame, reinterpret_cast<void*>(this));
    snkc_set_draw_snake(snake_data_, snakec_draw_snake, reinterpret_cast<void*>(this));
    snkc_set_draw_apple(snake_data_, snakec_draw_apple, reinterpret_cast<void*>(this));
    snkc_set_random(snake_data_, snakec_random, reinterpret_cast<void*>(this));
    snkc_set_game_over(snake_data_, snakec_game_over, reinterpret_cast<void*>(this));
    snkc_reset(snake_data_);
    redraw_ = true;
    prev_score_ = 0xffffffff;  // make it redraw on first frame
    wait_for_draw_ = false;
    s_.set_title(STRING(STRING_GAMEOVER));
}

void Snake::key_event(uint8_t key)
{
    switch( key )
    {
        case APP_KEY_SCROLL_CLOCKWISE:
            if ( wait_for_draw_ )
            {
                // skip/drop this input
                return;
            }
            direction_ = direction_ == 0 ? SNAKE_KEY_DOWN : direction_ - 1;
            switch (direction_)
            {
                case SNAKE_KEY_LEFT:
                    snkc_key_left(snake_data_);
                    wait_for_draw_ = true;
                    break;
                case SNAKE_KEY_UP:
                    snkc_key_up(snake_data_);
                    wait_for_draw_ = true;
                    break;
                case SNAKE_KEY_RIGHT:
                    snkc_key_right(snake_data_);
                    wait_for_draw_ = true;
                    break;
                case SNAKE_KEY_DOWN:
                    snkc_key_down(snake_data_);
                    wait_for_draw_ = true;
                    break;
            };
            break;

        case APP_KEY_SCROLL_ANTICLOCKWISE:
            direction_ = direction_ >= 4 ? SNAKE_KEY_RIGHT : direction_ + 1;
            if ( wait_for_draw_ )
            {
                // skip/drop this input
                return;
            }
            switch (direction_)
            {
                case SNAKE_KEY_LEFT:
                    snkc_key_left(snake_data_);
                    wait_for_draw_ = true;
                    break;
                case SNAKE_KEY_UP:
                    snkc_key_up(snake_data_);
                    wait_for_draw_ = true;
                    break;
                case SNAKE_KEY_RIGHT:
                    snkc_key_right(snake_data_);
                    wait_for_draw_ = true;
                    break;
                case SNAKE_KEY_DOWN:
                    snkc_key_down(snake_data_);
                    wait_for_draw_ = true;
                    break;
            };
            break;

        case APP_KEY_FORWARD:
            if ( direction_ != SNAKE_KEY_RIGHT )
            {
                direction_ = SNAKE_KEY_RIGHT;
                snkc_key_right(snake_data_);
                wait_for_draw_ = true;
            }
            else
            {
                direction_ = SNAKE_KEY_DOWN;
                snkc_key_down(snake_data_);
                wait_for_draw_ = true;
            }
            break;

        case APP_KEY_BACK:
            if ( direction_ != SNAKE_KEY_LEFT )
            {
                direction_ = SNAKE_KEY_LEFT;
                snkc_key_left(snake_data_);
                wait_for_draw_ = true;
            }
            else
            {
                direction_ = SNAKE_KEY_UP;
                snkc_key_up(snake_data_);
                wait_for_draw_ = true;
            }
            break;

        case APP_KEY_MENU:
            App::manager_end();
            break;
    }
}

void Snake::draw_begin_frame() 
{
    if ( !snake_canvas_ ) return;
    for ( size_t i = 0; i < (SNAKE_GRID_WIDTH * SNAKE_GRID_HEIGHT); i++ )
    {
        // move new nibble into old nibble and clear new nibble
        snake_canvas_[i] = (snake_canvas_[i] << 4) & 0xf0;
    }
}

void Snake::draw_snake(int16_t x, int16_t y) 
{
    if ( !snake_canvas_ ) return;
    size_t i = ((size_t)y * SNAKE_GRID_WIDTH) + (size_t)x;
    snake_canvas_[i] |= 0x1;
}

void Snake::draw_apple(int16_t x, int16_t y) 
{
    if ( !snake_canvas_ ) return;
    size_t i = ((size_t)y * SNAKE_GRID_WIDTH) + (size_t)x;
    snake_canvas_[i] |= 0x2;
}

void Snake::draw_end_frame(Adafruit_GFX &gfx) {
    size_t i = 0;
    if ( !snake_canvas_ ) return;
    for ( int16_t gy = 0; gy < SNAKE_GRID_HEIGHT; gy++ )
    {
        for ( int16_t gx = 0; gx < SNAKE_GRID_WIDTH; gx++ )
        {
            // check for a change on this spot
            if ( (snake_canvas_[i] & 0x0f) != (snake_canvas_[i] >> 4) & 0x0f )
            {
                int16_t x = SNAKE_GRID_OFFSET_X + SNAKE_BOARDER_INDENT + (gx * SNAKE_SQUARE_SIZE);
                int16_t y = SNAKE_GRID_OFFSET_Y + SNAKE_BOARDER_INDENT + (gy * SNAKE_SQUARE_SIZE);
                if ( snake_canvas_[i] & 0x02 )
                {
                    // apple
                    gfx.fillRect(x, y, SNAKE_SQUARE_SIZE, SNAKE_SQUARE_SIZE, 0xffff);
                    SnakeApple6x6::draw(x, y, gfx);
                    STRING_CACHE();
                    printf(STRING(CONSOLE_CURSOR_STRING), gy + 2, (gx + 1) * 2, STRING(BLOCK_APPLE));
                    STRING_CLEAR();
                }
                else if ( snake_canvas_[i] & 0x01 )
                {
                    // snake
                    gfx.fillRect(x, y, SNAKE_SQUARE_SIZE, SNAKE_SQUARE_SIZE, 0x0000);
                    STRING_CACHE();
                    printf(STRING(CONSOLE_CURSOR_STRING), gy + 2, (gx + 1) * 2, STRING(BLOCK_SQUARE));
                    STRING_CLEAR();
                }
                else
                {
                    // empty
                    gfx.fillRect(x, y, SNAKE_SQUARE_SIZE, SNAKE_SQUARE_SIZE, 0xffff);
                    STRING_CACHE();
                    printf(STRING(CONSOLE_CURSOR_STRING), gy + 2, (gx + 1) * 2, STRING(BLOCK_EMPTY));
                    STRING_CLEAR();
                }
            }

            // next square on canvas
            i++;
        }
    }
}

void Snake::draw(Adafruit_GFX &gfx)
{
    // redraw on game start
    if ( redraw_ )
    {
        BpodTitleBar::draw(gfx, STRING(STRING_SNAKE));
        gfx.fillRect(0, BpodTitleBar::view_y(gfx), gfx.width(), BpodTitleBar::view_height(gfx), 0xffff);
        gfx.drawRect(SNAKE_GRID_OFFSET_X, SNAKE_GRID_OFFSET_Y, SNAKE_BOARDER_WIDTH, SNAKE_BOARDER_HEIGTH, 0x001F);
        printf(STRING(CONSOLE_CLEAR));
        for ( size_t y = 0; y < (SNAKE_GRID_HEIGHT + 2); y++ )
        {
            if ( y == 0 || y == (SNAKE_GRID_HEIGHT + 1) )
            {
                printf(STRING(GRID_ASCII_LEFT_CORNER));
                for ( size_t x = 0; x < (SNAKE_GRID_WIDTH * 2); x++)
                {
                    printf(STRING(GRID_ASCII_TOP_BOTTOM_WALL));
                }
                printf(STRING(GRID_ASCII_RIGHT_CORNER));
            }
            else
            {
                printf(STRING(GRID_ASCII_LEFT_WALL));
                for ( size_t x = 0; x < (SNAKE_GRID_WIDTH * 2); x++)
                {
                    printf(STRING(GRID_ASCII_EMPTY));
                }
                printf(STRING(GRID_ASCII_RIGHT_WALL));
            }
        }
        redraw_ = false;
    }

    // do stuff (redraw grid if needed)
    static uint32_t last_tick = 0;
    uint32_t tick = (uint32_t)millis();
    if ( (tick - last_tick) > ((uint32_t)(1000/15)) )
    {
        last_tick = tick;
        snkc_tick(snake_data_);
        draw_end_frame(gfx);
        wait_for_draw_ = false;
    }

    // redraw score if its changed
    uint32_t score = snkc_get_score(snake_data_) * 100;
    if ( prev_score_ != score )
    {
        gfx.setTextColor(0x0000, 0xffff);
        int16_t text_height = 8;
        int16_t text_width = 8 * 6;
        gfx.setCursor(gfx.width() - text_width - 1, gfx.height() - text_height - 1);
        gfx.setTextSize(1, 1);
        char score_text[20];
        sprintf(score_text, STRING(FMT_SCORE), score);
        gfx.print(score_text);
        printf(STRING(CONSOLE_POS_GAME_SCORE), 23, 1, score);
        prev_score_ = score;
    }
}

void Snake::end(void)
{
    snkc_fini(snake_data_);
    delete snake_canvas_;
    snake_canvas_ = nullptr;
}
