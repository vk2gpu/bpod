#include "GameMenu.hpp"

#include <App/Snake.hpp>
#include <App/Tetris.hpp>
#include <ctf.h>
#include <score.h>

uint8_t rng_part(uint16_t value, uint8_t i)
{
    if (i & 0x1) {
        return (uint8_t)((value >> 8) & 0xff);
    }
    return (uint8_t)(value & 0xff);
}

uint8_t rng_u8(uint8_t i)
{
    uint8_t rng_value = ((uint8_t)random());
    rng_value ^= ((uint8_t)analogRead(i & 0x3));
    rng_value ^= rng_part((uint16_t)millis(), i >> 1);
    return rng_value;
}

void rng_init()
{
    uint16_t seed = 0;
    uint8_t *seed_part = (uint8_t*)&seed;
    seed_part[0] = rng_u8(5);
    seed_part[1] = rng_u8(6);
    randomSeed(seed);
}

void rngcpy(void *dst, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++) {
        ((uint8_t*)dst)[i] = rng_u8(i);
    }
}

Snake snake;
Tetris tetris;

static bool init_rng = true;
void GameMenu::begin(BpodMenu &menu) {
    menu.set_title("Games");
    menu.add("Snake", [](){ App::manager_begin(snake); });
    menu.add("Tetris", [](){ App::manager_begin(tetris); });
    if ( init_rng )
    {
        init_rng = false;
        rng_init();
    }
    set_rng(rngcpy);

    CTF_OFF_MACRO
}

void GameMenu::end()
{
    CTF_ON_MACRO
    Menu::end();
}
