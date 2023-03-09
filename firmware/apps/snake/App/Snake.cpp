#include "Snake.hpp"

#define SNAKE_KEY_RIGHT     (0)
#define SNAKE_KEY_UP        (1)
#define SNAKE_KEY_LEFT      (2)
#define SNAKE_KEY_DOWN      (3)

void SNKC_API snakec_begin_frame(void *ctx)
{
    Snake &snake = *reinterpret_cast<Snake*>(ctx);
    Adafruit_GFX &gfx = *snake.draw_gfx;
    gfx.fillScreen(0xffff);
    gfx.setTextColor(0x0000);
    gfx.setCursor(0, 0);
    gfx.setTextSize(1, 1);
    char score_text[20];
    sprintf(score_text, "%8d", snkc_get_score(snake.snake_data) * 100);
    gfx.print(score_text);
    gfx.drawRect(SNAKE_GRID_OFFSET_X, SNAKE_GRID_OFFSET_Y, SNAKE_BOARDER_WIDTH, SNAKE_BOARDER_HEIGTH, 0x0000);
}

void SNKC_API snakec_end_frame(void *ctx)
{

}

void SNKC_API snakec_draw_snake(void *ctx, int16_t x,int16_t y)
{
    Snake &snake = *reinterpret_cast<Snake*>(ctx);
    Adafruit_GFX &gfx = *snake.draw_gfx;
    x = SNAKE_GRID_OFFSET_X + SNAKE_BOARDER_INDENT + (x * SNAKE_SQUARE_SIZE);
    y = SNAKE_GRID_OFFSET_Y + SNAKE_BOARDER_INDENT + (y * SNAKE_SQUARE_SIZE);
    gfx.fillRect(x, y, SNAKE_SQUARE_SIZE, SNAKE_SQUARE_SIZE, 0x0000);
}

void SNKC_API snakec_draw_apple(void *ctx, int16_t x,int16_t y)
{
    Snake &snake = *reinterpret_cast<Snake*>(ctx);
    Adafruit_GFX &gfx = *snake.draw_gfx;
    x = SNAKE_GRID_OFFSET_X + SNAKE_BOARDER_INDENT + (x * SNAKE_SQUARE_SIZE);
    y = SNAKE_GRID_OFFSET_Y + SNAKE_BOARDER_INDENT + (y * SNAKE_SQUARE_SIZE);
    gfx.fillCircle(x+((SNAKE_SQUARE_SIZE-1)/2), y+((SNAKE_SQUARE_SIZE-1)/2), ((SNAKE_SQUARE_SIZE-1)/2) + 1, 0x0000);
}

int16_t SNKC_API snakec_random(void *ctx, int16_t min, int16_t max)
{
    return random(min, max + 1);
}

static void SNKC_API snakec_game_over(void *ctx, uint16_t score)
{
    App::manager_end();
}

void Snake::begin(void)
{
    direction = SNAKE_KEY_UP;
    snkc_init(snake_data, sizeof(snake_data));
    snkc_set_grid(snake_data, SNAKE_GRID_WIDTH, SNAKE_GRID_HEIGHT);
    snkc_set_draw_clear(snake_data, snakec_begin_frame, reinterpret_cast<void*>(this));
    snkc_set_draw_snake(snake_data, snakec_draw_snake, reinterpret_cast<void*>(this));
    snkc_set_draw_apple(snake_data, snakec_draw_apple, reinterpret_cast<void*>(this));
    snkc_set_random(snake_data, snakec_random, reinterpret_cast<void*>(this));
    snkc_set_game_over(snake_data, snakec_game_over, reinterpret_cast<void*>(this));
    snkc_reset(snake_data);
}

void Snake::key_event(uint8_t key)
{
    switch( key )
    {
        case APP_KEY_SCROLL_CLOCKWISE:
            direction = direction == 0 ? SNAKE_KEY_DOWN : direction - 1;
            switch (direction)
            {
                case SNAKE_KEY_LEFT:
                    snkc_key_left(snake_data);
                    break;
                case SNAKE_KEY_UP:
                    snkc_key_up(snake_data);
                    break;
                case SNAKE_KEY_RIGHT:
                    snkc_key_right(snake_data);
                    break;
                case SNAKE_KEY_DOWN:
                    snkc_key_down(snake_data);
                    break;
            };
            break;

        case APP_KEY_SCROLL_ANTICLOCKWISE:
            direction = direction >= 4 ? SNAKE_KEY_RIGHT : direction + 1;
            switch (direction)
            {
                case SNAKE_KEY_LEFT:
                    snkc_key_left(snake_data);
                    break;
                case SNAKE_KEY_UP:
                    snkc_key_up(snake_data);
                    break;
                case SNAKE_KEY_RIGHT:
                    snkc_key_right(snake_data);
                    break;
                case SNAKE_KEY_DOWN:
                    snkc_key_down(snake_data);
                    break;
            };
            break;

        case APP_KEY_OK:
            break;

        case APP_KEY_MENU:
            App::manager_end();
            break;
    }
}

void Snake::draw(Adafruit_GFX &gfx)
{
    static uint32_t last_tick = 0;
    uint32_t tick = (uint32_t)millis();
    draw_gfx = &gfx;
    if ( (tick - last_tick) > ((uint32_t)(1000/15)) )
    {
        last_tick = tick;
        snkc_tick(snake_data);
        snakec_end_frame(reinterpret_cast<void*>(this));
    }
    draw_gfx = nullptr;
}

void Snake::end(void)
{
    snkc_fini(snake_data);
}
