#pragma GCC optimize ("-O3")

#include "libgame.h"
#include "graphics.h"
#include "binary.h"
#include "controls.h"
#include "music.h"
#include "tunes.h"

//#ifdef FRAME_BUFFER

enum
{
    LEFT,
    UP,
    RIGHT,
    DOWN,
};

#define CCW(d) (((d) + 4 - 1) % 4)
#define CW(d) (((d) + 1) % 4)

#define MW 32
#define MH 32

const uint8_t maze[] PROGMEM = {
    B11111111, B11111111, B11111111, B11111111,
    B10000000, B10000000, B00101110, B00010001,
    B10111110, B10111110, B11100110, B11010111,
    B11100000, B11101100, B10101110, B11010001,
    B10001110, B10001110, B10111000, B11010101,
    B10101110, B11101110, B10111110, B11111101,
    B10100110, B00001110, B10010110, B11101001,
    B10101110, B11101110, B11010110, B00001011,
    B10100110, B11000110, B11000110, B11101001,
    B10111110, B11010110, B11010110, B11111111,
    B10000000, B00010000, B00010000, B00000001,
    B11011111, B11010110, B10110110, B11111101,
    B10010110, B11010110, B10110111, B11110111,
    B10110110, B11010110, B10100001, B11110111,
    B10110110, B11000110, B11111100, B11000011,
    B10000110, B11101100, B11111111, B11111001,
    B11011110, B00111000, B11000000, B11011101,
    B11000000, B11100010, B11110110, B11000001,
    B10011110, B11001110, B10110110, B11111101,
    B11000110, B11111110, B10110110, B11000001,
    B10011110, B11000110, B10111110, B11111101,
    B10110110, B11101110, B10010110, B00010001,
    B10100110, B11000110, B11000110, B11010101,
    B10101110, B11010110, B11010110, B11111101,
    B10000000, B00010000, B00000000, B00000001,
    B11110110, B11010110, B11111110, B11111101,
    B11010110, B11010110, B11011110, B11010111,
    B11010110, B11010110, B11000000, B11000111,
    B11010110, B11000110, B11111100, B11011111,
    B11010110, B01101100, B11011110, B11011001,
    B11000111, B01111000, B11000000, B11011101,
    B11111111, B11111111, B11111111, B11111111,
};

static const uint8_t sidewall0Lines[] PROGMEM = {
    B10000000, B00000000, 
    B11000000, B00000000, 
    B11000000, B00000000, 
    B11010000, B00000000, 
    B11011000, B00000000, 
    B11011100, B00000000, 
    B11011110, B00000000, 
    B11011111, B00000000, 
    B11011111, B10000000, 
    B00011111, B10000000, 
    B00011111, B10000000, 
    B11001111, B10000000, 
    B11100011, B10000000, 
    B11111001, B10000000, 
    B11111100, B00000000, 
    B11111101, B00000000, 
    B11111101, B10000000, 
    B11111101, B10000000, 
    B11111101, B10000000, 
    B11111101, B10000000, 
    B00011101, B10000000, 
    B00000001, B10000000, 
    B11000000, B00000000, 
    B11011100, B00000000, 
    B11011111, B10000000, 
    B11011111, B10000000, 
    B11011111, B10000000, 
    B11011111, B10000000, 
    B11011111, B10000000, 
    B11011111, B10000000, 
    B11010000, B00000000, 
    B00000000, B00000000, 
    B00001101, B10000000, 
    B11111101, B10000000, 
    B11111101, B10000000, 
    B11111101, B10000000, 
    B11111101, B10000000, 
    B11111101, B10000000, 
    B11111101, B10000000, 
    B11111100, B00000000, 
    B11111000, B00000000, 
    B11000001, B10000000, 
    B00000111, B10000000, 
    B00011111, B10000000, 
    B11011111, B10000000, 
    B11011111, B10000000, 
    B11011111, B10000000, 
    B11011111, B00000000, 
    B11011100, B00000000, 
    B11011000, B10000000, 
    B11010001, B10000000, 
    B11000101, B10000000, 
    B10001101, B10000000, 
    B00111101, B10000000, 
    B01111101, B10000000, 
    B11111101, B10000000, 
    B11111101, B00000000, 
    B11111100, B00000000, 
    B11111100, B00000000, 
    B11111000, B00000000, 
    B11110000, B00000000, 
    B11100000, B00000000, 
    B11000000, B00000000, 
    B10000000, B00000000, 
};
static const game_sprite sidewall0sprite PROGMEM = {9, 64, sidewall0Lines};

static const uint8_t sidewall1Lines[] PROGMEM = {
    B10000000, 
    B11000000, 
    B11000000, 
    B11010000, 
    B11011000, 
    B11011100, 
    B01011110, 
    B00011111, 
    B10011111, 
    B11001111, 
    B11110111, 
    B11111011, 
    B11111000, 
    B01111011, 
    B00011011, 
    B11001011, 
    B11010011, 
    B11011100, 
    B11011111, 
    B11011111, 
    B11011111, 
    B00011111, 
    B00000000, 
    B11111011, 
    B11111011, 
    B11111011, 
    B11111011, 
    B11111000, 
    B11000001, 
    B00001111, 
    B00011111, 
    B11011111, 
    B11011111, 
    B11011110, 
    B11011001, 
    B11000011, 
    B10001011, 
    B00011011, 
    B01111011, 
    B11111010, 
    B11111000, 
    B11111000, 
    B11110000, 
    B11100000, 
    B11000000, 
    B10000000, 
};
static const game_sprite sidewall1sprite PROGMEM = {8, 46, sidewall1Lines};

static const uint8_t sidewall2Lines[] PROGMEM = {
    B10000000, 
    B11000000, 
    B11000000, 
    B11010000, 
    B00011000, 
    B11011100, 
    B11101110, 
    B11110010, 
    B11111000, 
    B00111010, 
    B11000010, 
    B11011000, 
    B11011110, 
    B11011110, 
    B00000000, 
    B11111010, 
    B11111010, 
    B11111010, 
    B11100000, 
    B00011110, 
    B11011110, 
    B11011000, 
    B11010010, 
    B11001010, 
    B10011000, 
    B01111000, 
    B11110000, 
    B11100000, 
    B11000000, 
    B10000000, 
};
static const game_sprite sidewall2sprite PROGMEM = {7, 30, sidewall2Lines};

static const uint8_t sidewall3Lines[] PROGMEM = {
    B10000000, 
    B00000000, 
    B10100000, 
    B11000000, 
    B01101000, 
    B10110000, 
    B10111000, 
    B00000000, 
    B11101000, 
    B11100000, 
    B11011000, 
    B00101000, 
    B10000000, 
    B10100000, 
    B01000000, 
    B10000000, 
};
static const game_sprite sidewall3sprite PROGMEM = {5, 16, sidewall3Lines};

static const uint8_t frontwall1Lines[] PROGMEM = {
    B11011111, B10000000, 
    B11011111, B10000000, 
    B11011111, B10000000, 
    B11011111, B10000000, 
    B11011111, B10000000, 
    B11011111, B10000000, 
    B00000000, B00000000, 
    B00000000, B00000000, 
    B11111101, B10000000, 
    B11111101, B10000000, 
    B11111101, B10000000, 
    B11111101, B10000000, 
    B11111101, B10000000, 
    B11111101, B10000000, 
    B00000000, B00000000, 
    B00000000, B00000000, 
    B11011111, B10000000, 
    B11011111, B10000000, 
    B11011111, B10000000, 
    B11011111, B10000000, 
    B11011111, B10000000, 
    B11011111, B10000000, 
    B00000000, B00000000, 
    B00000000, B00000000, 
    B11111101, B10000000, 
    B11111101, B10000000, 
    B11111101, B10000000, 
    B11111101, B10000000, 
    B11111101, B10000000, 
    B11111101, B10000000, 
    B00000000, B00000000, 
    B00000000, B00000000, 
    B11011111, B10000000, 
    B11011111, B10000000, 
    B11011111, B10000000, 
    B11011111, B10000000, 
    B11011111, B10000000, 
    B11011111, B10000000, 
    B00000000, B00000000, 
    B00000000, B00000000, 
    B11111101, B10000000, 
    B11111101, B10000000, 
    B11111101, B10000000, 
    B11111101, B10000000, 
    B11111101, B10000000, 
    B11111101, B10000000, 
};
static const game_sprite frontwall1sprite PROGMEM = {9, 46, frontwall1Lines};

static const uint8_t frontwall2Lines[] PROGMEM = {
    B11011111, 
    B11011111, 
    B11011111, 
    B11011111, 
    B00000000, 
    B11111011, 
    B11111011, 
    B11111011, 
    B11111011, 
    B00000000, 
    B11011111, 
    B11011111, 
    B11011111, 
    B11011111, 
    B00000000, 
    B11111011, 
    B11111011, 
    B11111011, 
    B11111011, 
    B00000000, 
    B11011111, 
    B11011111, 
    B11011111, 
    B11011111, 
    B00000000, 
    B11111011, 
    B11111011, 
    B11111011, 
    B11111011, 
    B11111011, 
};
static const game_sprite frontwall2sprite PROGMEM = {8, 30, frontwall2Lines};

static const uint8_t frontwall3Lines[] PROGMEM = {
    B10111110, 
    B00000000, 
    B11111010, 
    B11111010, 
    B00000000, 
    B10111110, 
    B10111110, 
    B00000000, 
    B11111010, 
    B11111010, 
    B00000000, 
    B10111110, 
    B10111110, 
    B00000000, 
    B11111010, 
    B11111010, 
};
static const game_sprite frontwall3sprite PROGMEM = {7, 16, frontwall3Lines};

static const uint8_t frontwall4Lines[] PROGMEM = {
    B11111100, 
    B11011100, 
    B11110100, 
    B10111100, 
    B01101000, 
    B11111100, 
};
static const game_sprite frontwall4sprite PROGMEM = {6, 6, frontwall4Lines};


struct Maze3DData
{
    int8_t x, y, dir;
};
static Maze3DData* data;

static bool IsWall(uint8_t x, uint8_t y)
{
    uint16_t off = x / 8 + y * MW / 8;
    uint8_t walls = pgm_read_byte(maze + off);
    return walls & (1 << (7 - x % 8));
}

static int8_t GetDx(uint8_t dir)
{
    return dir == LEFT ? -1 : dir == RIGHT ? 1 : 0;
}

static int8_t GetDy(uint8_t dir)
{
    return dir == UP ? -1 : dir == DOWN ? 1 : 0;
}

#define COLOR_SIDE RED_DARK
#define COLOR_FRONT RED

static void Maze3D_draw()
{
    game_clear_screen();
    // draw walls
    int8_t dx = GetDx(data->dir);
    int8_t dy = GetDy(data->dir);
    int8_t left = CCW(data->dir);
    int8_t right = CW(data->dir);
    int8_t dxl = GetDx(left);
    int8_t dyl = GetDy(left);
    int8_t dxr = GetDx(right);
    int8_t dyr = GetDy(right);
    if (IsWall(data->x + dx, data->y + dy))
    {
        game_draw_sprite(&frontwall1sprite, 9, 9, COLOR_FRONT);
        game_draw_sprite(&frontwall1sprite, 15, 9, COLOR_FRONT);
        game_draw_sprite(&frontwall1sprite, 23, 9, COLOR_FRONT);
        game_draw_sprite(&frontwall1sprite, 32, 9, COLOR_FRONT);
        game_draw_sprite(&frontwall1sprite, 37, 9, COLOR_FRONT);
        game_draw_sprite(&frontwall1sprite, 46, 9, COLOR_FRONT);
    }
    else
    {
        if (IsWall(data->x + 2 * dx, data->y + 2 * dy))
        {
            game_draw_sprite(&frontwall2sprite, 17, 17, COLOR_FRONT);
            game_draw_sprite(&frontwall2sprite, 24, 17, COLOR_FRONT);
            game_draw_sprite(&frontwall2sprite, 32, 17, COLOR_FRONT);
            game_draw_sprite(&frontwall2sprite, 39, 17, COLOR_FRONT);
        }
        else
        {
            if (IsWall(data->x + 3 * dx, data->y + 3 * dy))
            {
                game_draw_sprite(&frontwall3sprite, 24, 24, COLOR_FRONT);
                game_draw_sprite(&frontwall3sprite, 30, 24, COLOR_FRONT);
                game_draw_sprite(&frontwall3sprite, 33, 24, COLOR_FRONT);
            }
            else
            {
                if (IsWall(data->x + 4 * dx, data->y + 4 * dy))
                    game_draw_sprite(&frontwall4sprite, 29, 29, COLOR_FRONT);
                if (IsWall(data->x + dxl + 3 * dx, data->y + dyl + 3 * dy))
                    game_draw_sprite(&sidewall3sprite, 24, 24, COLOR_SIDE);
                else if (IsWall(data->x + dxl + 4 * dx, data->y + dyl + 4 * dy))
                    game_draw_sprite(&frontwall4sprite, 24, 29, COLOR_FRONT);
                if (IsWall(data->x + dxr + 3 * dx, data->y + dyr + 3 * dy))
                    game_draw_sprite(&sidewall3sprite, 35, 24, COLOR_SIDE, SPRITE_MIRROR_H);
                else if (IsWall(data->x + dxr + 4 * dx, data->y + dyr + 4 * dy))
                    game_draw_sprite(&frontwall4sprite, 34, 29, COLOR_FRONT);
            }
            if (IsWall(data->x + dxl + 2 * dx, data->y + dyl + 2 * dy))
                game_draw_sprite(&sidewall2sprite, 17, 17, COLOR_SIDE);
            else if (IsWall(data->x + dxl + 3 * dx, data->y + dyl + 3 * dy))
                game_draw_sprite(&frontwall3sprite, 17, 24, COLOR_FRONT);
            if (IsWall(data->x + dxr + 2 * dx, data->y + dyr + 2 * dy))
                game_draw_sprite(&sidewall2sprite, 40, 17, COLOR_SIDE, SPRITE_MIRROR_H);
            else if (IsWall(data->x + dxr + 3 * dx, data->y + dyr + 3 * dy))
                game_draw_sprite(&frontwall3sprite, 40, 24, COLOR_FRONT);
        }
        if (IsWall(data->x + dxl + dx, data->y + dyl + dy))
            game_draw_sprite(&sidewall1sprite, 9, 9, COLOR_SIDE);
        else if (IsWall(data->x + dxl + 2 * dx, data->y + dyl + 2 * dy))
            game_draw_sprite(&frontwall2sprite, 9, 17, COLOR_FRONT);
        if (IsWall(data->x + dxr + dx, data->y + dyr + dy))
            game_draw_sprite(&sidewall1sprite, 47, 9, COLOR_SIDE, SPRITE_MIRROR_H);
        else if (IsWall(data->x + dxr + 2 * dx, data->y + dyr + 2 * dy))
            game_draw_sprite(&frontwall2sprite, 47, 17, COLOR_FRONT);
    }
    if (IsWall(data->x + dxl, data->y + dyl))
        game_draw_sprite(&sidewall0sprite, 0, 0, COLOR_SIDE);
    else if (IsWall(data->x + dxl + dx, data->y + dyl + dy))
        game_draw_sprite(&frontwall1sprite, 0, 9, COLOR_FRONT);
    if (IsWall(data->x + dxr, data->y + dyr))
        game_draw_sprite(&sidewall0sprite, 55, 0, COLOR_SIDE, SPRITE_MIRROR_H);
    else if (IsWall(data->x + dxr + dx, data->y + dyr + dy))
        game_draw_sprite(&frontwall1sprite, 55, 9, COLOR_FRONT);
    // draw map
    const int mapX = 6;
    const int mapY = 6;
    for (int8_t dx = -4 ; dx <= 4 ; ++dx)
    {
        int8_t x = data->x + dx;
        for (int8_t dy = -4 ; dy <= 4 ; ++dy)
        {
            int8_t y = data->y + dy;
            if (x < 0 || x >= MW || y < 0 || y >= MH
                || IsWall(x, y))
            {
                game_draw_pixel(mapX + dx, mapY + dy, BLUE);
            }
            else
            {
                game_draw_pixel(mapX + dx, mapY + dy, BLACK);
            }
            
        }
    }
    // draw player on map
    game_draw_pixel(mapX, mapY, GREEN);
}

static void Maze3D_prepare()
{
    game_enable_frame_buffer();
    game_set_background(BLACK);
    game_set_ups(10);
    data->y = 2;
    data->x = 7;
    data->dir = DOWN;

    Maze3D_draw();
}

static void Maze3D_render()
{
}

static void Maze3D_update(unsigned long delta)
{
    bool upd = false;
 
    if (game_is_button_pressed(BUTTON_UP))
    {
        int8_t x = data->x + GetDx(data->dir);
        int8_t y = data->y + GetDy(data->dir);
        if (!IsWall(x, y))
        {
            upd = true;
            data->x = x;
            data->y = y;
        }
    }
    else if (game_is_button_pressed(BUTTON_LEFT))
    {
        upd = true;
        data->dir = CCW(data->dir);
    }
    else if (game_is_button_pressed(BUTTON_RIGHT))
    {
        upd = true;
        data->dir = CW(data->dir);
    }

    if (upd)
    {
        Maze3D_draw();
    }
}

const game_instance Maze3D PROGMEM = {
    "Maze3D",
    Maze3D_prepare,
    Maze3D_render,
    Maze3D_update,
    sizeof(Maze3DData),
    (void**)(&data)
};

//#endif