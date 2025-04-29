#include <math.h>
#pragma once

#define SCREEN_HEIGHT 900 // pixels
#define SCREEN_WIDTH 672  //  pixels
#define BOARD_HEIGHT 672  //  pixels
#define BOARD_WIDTH 672   //  pixels

#define RA(min, max) ((min) + rand() % ((max) - (min) + 1)) // random integer number
#define MAX(a, b) ((a) > (b) ? (a) : (b))                   // max value of two numbers

#define CELL_SIZE 32                           // pixels
#define GRID_WIDTH (BOARD_WIDTH / CELL_SIZE)   // size of grid width
#define GRID_HEIGHT (BOARD_HEIGHT / CELL_SIZE) // size of grid height
#define INITIAL_SNAKE_LENGTH 5                 // starting snake's length
#define INITIAL_SNAKE_X GRID_WIDTH / 2         // starting snake's x position
#define INITIAL_SNAKE_Y GRID_HEIGHT / 2        // starting snake's x position

#define MAX_SNAKE_LENGTH 100         // max snake length
#define SPEED_UP_TIMER 10            // every timer value seconds game speeds up
#define SPEED_UP -0.02               // value of speeding up in ms
#define SLOW_DOWN 0.1                // value of slowing down in ms
#define BONUS_SPAWN_TIMER RA(15, 30) // random bonus spawn timer (currently <15;30> seconds)
#define SHORTENS 3                   // number of segments that are cutten of snake when bonus is eaten
#define INITIAL_SPEED_TIMER 0.2      // default speed of the game in ms
#define MINIMUM_SPEED_TIMER 0.1      // minimum speed for snake in ms
#define BONUS_TIME 20                // time that bonus is displaying on the board in seconds
#define BONUS_EATEN_SCORE 30         // number of points for eating bonus (red dot)
#define FOOD_EATEN_SCORE 10          // number of points for eating food (blue dot)

// structs

struct Position {
    int x, y;
};

struct Snake {
    Position segments[MAX_SNAKE_LENGTH];
    int length;
    int dx;
    int dy;
};

struct Dot {
    Position position;
};

struct GameState {
    Snake snake;
    Dot food;
    Dot *bonus;
    int score;
    int timer;
    int bonus_timer;
    int last_bonus_timer;
    int bonus_spawn_timer;
    double speed_timer;
    bool gameEnded;
    bool exit;
    bool isBonusActive;
};
