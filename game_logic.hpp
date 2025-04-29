#include "./config.hpp"
#include "./draw.hpp"
#include <SDL2/SDL.h>
#pragma once

// checking game events

// checking if snake's body isn't colliding with (x,y) of the dot
bool checkDotCollision(int x, int y, Snake &snake) {
    for (int i = 0; i < snake.length; i++) {
        if (x == snake.segments[i].x && y == snake.segments[i].y) {
            return true;
        }
    }
    return false;
}

// checking if snake's head is colliding with the edge
bool checkEdgeCollision(Snake &snake) {
    if (snake.segments[0].x == 0 || snake.segments[0].x == GRID_WIDTH - 1 ||
        snake.segments[0].y == 0 || snake.segments[0].y == GRID_HEIGHT - 1) {
        return true;
    }
    return false;
}

// checking if snake did not hit himself
bool checkSelfCollision(Snake &snake) {
    for (int i = 1; i < snake.length; i++) {
        if (snake.segments[0].x == snake.segments[i].x && snake.segments[0].y == snake.segments[i].y) {
            return true;
        }
    }

    return false;
}

// checking if food was eaten
bool checkFoodEaten(Snake &snake, Dot &food) {
    if (snake.segments[0].x == food.position.x && snake.segments[0].y == food.position.y) {
        return true;
    }

    return false;
}

// checking if bonus was eaten
bool checkBonusEaten(GameState &state, Snake &snake, Dot *bonus) {
    // if bonus is eaten delete it and change bonus active value
    if (snake.segments[0].x == bonus->position.x && snake.segments[0].y == bonus->position.y) {
        delete state.bonus;
        state.bonus = NULL;
        state.isBonusActive = false;
        return true;
    }

    return false;
}

void placeRandomFood(Dot &food, Snake &snake) {
    int tempX = rand() % (GRID_WIDTH - 2) + 1;
    int tempY = rand() % (GRID_HEIGHT - 2) + 1;

    if (!checkDotCollision(tempX, tempY, snake)) {
        food.position.x = tempX;
        food.position.y = tempY;
    }
}

void placeRandomBonus(Snake &snake, GameState &state) {
    int tempX = rand() % (GRID_WIDTH - 2) + 1;
    int tempY = rand() % (GRID_HEIGHT - 2) + 1;

    if (!checkDotCollision(tempX, tempY, snake)) {
        // creating and setting values for new bonus
        state.bonus = new Dot;
        state.bonus->position.x = tempX;
        state.bonus->position.y = tempY;
        state.isBonusActive = true;
        state.bonus_timer = BONUS_TIME;
    }
}

void autoTurnOnEdge(Snake &snake) {
    // top edge, up movement
    if (snake.segments[0].y == 1 && snake.dy == -1) {
        if (snake.segments[0].x < GRID_WIDTH - 2) { // right turn
            snake.dx = 1;
            snake.dy = 0;
        } else { // left turn
            snake.dx = -1;
            snake.dy = 0;
        }
    }
    // bottom edge, down movement
    else if (snake.segments[0].y == GRID_HEIGHT - 2 && snake.dy == 1) {
        if (snake.segments[0].x < GRID_WIDTH - 2) { // turn right
            snake.dx = 1;
            snake.dy = 0;
        } else { // turn left
            snake.dx = -1;
            snake.dy = 0;
        }
    }
    // right edge, right movement
    else if (snake.segments[0].x == GRID_WIDTH - 2 && snake.dx == 1) {
        if (snake.segments[0].y < GRID_HEIGHT - 2) { // turn right
            snake.dx = 0;
            snake.dy = 1;
        } else { // turn left
            snake.dx = 0;
            snake.dy = -1;
        }
    }
    // left edge, left movement
    else if (snake.segments[0].x == 1 && snake.dx == -1) {
        if (snake.segments[0].y > 1) { // turn up
            snake.dx = 0;
            snake.dy = -1;
        } else { // turn down
            snake.dx = 0;
            snake.dy = 1;
        }
    }
}

void handleBonusEffect(GameState &state) {
    state.score += BONUS_EATEN_SCORE;

    if (RA(0, 1)) {
        state.speed_timer += SLOW_DOWN;
    } else {
        if (state.snake.length > INITIAL_SNAKE_LENGTH + SHORTENS) {
            state.snake.length -= SHORTENS;
        } else {
            state.snake.length = INITIAL_SNAKE_LENGTH;
        }
    }

    state.isBonusActive = false;
}

// running game for the first time or restarting game by 'n'
void gameInit(GameState &state, SDL_Renderer *renderer) {
    // set default values for game initialize
    SDL_RenderClear(renderer);
    state.bonus = NULL;
    state.snake.length = INITIAL_SNAKE_LENGTH;
    state.snake.dx = 1;
    state.snake.dy = 0;
    state.score = 0;
    state.timer = 0;
    state.bonus_timer = BONUS_TIME;
    state.bonus_spawn_timer = RA(15, 30);
    state.last_bonus_timer = 0;
    state.speed_timer = INITIAL_SPEED_TIMER;
    state.gameEnded = false;
    state.exit = false;
    state.isBonusActive = false;

    // set start (x,y)
    int startX = INITIAL_SNAKE_X;
    int startY = INITIAL_SNAKE_Y;

    // set start snake position
    for (int i = 0; i < INITIAL_SNAKE_LENGTH; i++) {
        state.snake.segments[i].x = startX - i;
        state.snake.segments[i].y = startY;
    }

    // place first food on the board
    placeRandomFood(state.food, state.snake);
}

void handleEvents(GameState &state, SDL_Renderer *renderer) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                state.gameEnded = true;
            } else if (event.key.keysym.sym == SDLK_n) {
                if (state.bonus != NULL) { // if bonus is active delete it for new game
                    delete state.bonus;
                    state.bonus = NULL;
                }

                gameInit(state, renderer); // start new game

            } else if (event.key.keysym.sym == SDLK_UP && state.snake.dy == 0) {
                state.snake.dx = 0;
                state.snake.dy = -1;
            } else if (event.key.keysym.sym == SDLK_DOWN && state.snake.dy == 0) {
                state.snake.dx = 0;
                state.snake.dy = 1;
            } else if (event.key.keysym.sym == SDLK_LEFT && state.snake.dx == 0) {
                state.snake.dx = -1;
                state.snake.dy = 0;
            } else if (event.key.keysym.sym == SDLK_RIGHT && state.snake.dx == 0) {
                state.snake.dx = 1;
                state.snake.dy = 0;
            }
            break;

        case SDL_QUIT:
            state.gameEnded = true;
            break;
        }
    }
}

void handleEndScreenEvents(GameState &state, SDL_Renderer *renderer) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                state.gameEnded = true;
            } else if (event.key.keysym.sym == SDLK_n) {
                if (state.bonus != NULL) { // if bonus is active delete it for new game
                    delete state.bonus;
                    state.bonus = NULL;
                }

                gameInit(state, renderer);
            }
            break;
        case SDL_QUIT:
            state.gameEnded = true;
            break;
        }
    }
}

// updates for timers and snake

void updateSnakeMove(Snake &snake) {
    // move snake's segments segments
    for (int i = snake.length - 1; i > 0; i--) {
        snake.segments[i] = snake.segments[i - 1];
    }

    // move snake's head
    snake.segments[0].x += snake.dx;
    snake.segments[0].y += snake.dy;
}

void updateSnakeState(GameState &state, int &t1_snake, int t2) {
    float delta_snake;
    delta_snake = (t2 - t1_snake) * 0.001; // calculate time in seconds

    if (delta_snake >= state.speed_timer) { // by default every 200ms
        autoTurnOnEdge(state.snake);
        updateSnakeMove(state.snake);

        t1_snake = t2;
    }
}

void updateTimers(GameState &state, int &t1_timer, int t2) {
    float delta_timer = (t2 - t1_timer) * 0.001; // calculate time in seconds

    if (delta_timer >= 1.0f) {
        state.timer++;

        if (state.timer % SPEED_UP_TIMER == 0 && state.timer > 0 && state.speed_timer >= MINIMUM_SPEED_TIMER) { // every 20 seconds snake speeds up
            state.speed_timer += SPEED_UP;
        }

        if (state.isBonusActive && state.bonus_timer > 0) {
            state.bonus_timer--;
        } else if (!state.isBonusActive) {
            state.last_bonus_timer++; // counting to the next bonus
        }

        t1_timer = t2;
    }
}

bool checkPossibleMove(int &t1_move, int t2) {
    float delta_timer = (t2 - t1_move) * 0.001; // calculate time in seconds

    if (delta_timer >= 0.175) { // move possible every 175ms
        t1_move = t2;
        return true;
    }
    return false;
}

void handleFoodEaten(GameState &state) {
    if (checkFoodEaten(state.snake, state.food)) {
        state.snake.length++;
        state.snake.segments[state.snake.length - 1].x = state.snake.segments[state.snake.length - 2].x;
        state.snake.segments[state.snake.length - 1].y = state.snake.segments[state.snake.length - 2].y;

        state.score += FOOD_EATEN_SCORE;
        placeRandomFood(state.food, state.snake);
    }
}

void handleBonusEaten(GameState &state) {
    // if bonus is eaten handle the effect and delete it
    if (state.isBonusActive && checkBonusEaten(state, state.snake, state.bonus)) {
        handleBonusEffect(state);
        delete state.bonus;
        state.bonus = NULL;
        state.last_bonus_timer = 0;
        state.bonus_timer = RA(15, 30);

    }
    // if timer goes to 0 delete bonus
    else if (state.bonus_timer == 0) {
        if (state.bonus != NULL) {
            delete state.bonus;
            state.bonus = NULL;
            state.last_bonus_timer = 0;
            state.bonus_timer = RA(15, 30);
            state.isBonusActive = false;
        }
    }
}

void handleSelfCollision(GameState &state) {
    if (checkSelfCollision(state.snake)) {
        state.exit = true;
    }
}

void handleGameEvents(GameState &state) {
    handleSelfCollision(state);
    handleFoodEaten(state);
    handleBonusEaten(state);
}

void gameLoop(GameState &state, SDL_Renderer *renderer, SDL_Texture *head, SDL_Texture *body,
              SDL_Texture *tail, SDL_Texture *blueDot, SDL_Texture *redDot, SDL_Texture *border,
              SDL_Texture *bar, SDL_Texture *charset) {

    int t1_snake = SDL_GetTicks(), t1_timer = SDL_GetTicks(), t1_move = SDL_GetTicks(), t2;

    while (!state.gameEnded) {
        // drawing end screen
        if (state.exit) {
            drawEndScreen(state, renderer, charset);
            handleEndScreenEvents(state, renderer);
            continue;
        }

        t2 = SDL_GetTicks();

        updateTimers(state, t1_timer, t2);
        handleGameEvents(state);
        updateSnakeState(state, t1_snake, t2);

        if (checkPossibleMove(t1_move, t2)) {
            handleEvents(state, renderer);
        }

        if (state.isBonusActive == false && state.last_bonus_timer % state.bonus_spawn_timer == 0 && state.last_bonus_timer > 0) {
            placeRandomBonus(state.snake, state);
        }

        renderGame(state, renderer, head, body, tail, blueDot, redDot, border, bar, charset);
    }

    if (state.bonus != NULL) {
        delete state.bonus;
        state.bonus = NULL;
    }
}

void game(GameState &state) {
    // turn the cursor off
    SDL_ShowCursor(SDL_DISABLE);

    // creating window and renderer
    SDL_Window *window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // load textures
    SDL_Texture *head = loadBMPFromFile("/assets/snake_head_32x32.bmp", renderer);
    SDL_Texture *body = loadBMPFromFile("/assets/snake_body_32x32.bmp", renderer);
    SDL_Texture *tail = loadBMPFromFile("/assets/snake_tail_32x32.bmp", renderer);
    SDL_Texture *blueDot = loadBMPFromFile("/assets/blue_dot_32x32.bmp", renderer);
    SDL_Texture *redDot = loadBMPFromFile("/assets/red_dot_32x32.bmp", renderer);
    SDL_Texture *border = loadBMPFromFile("/assets/border_32x32.bmp", renderer);
    SDL_Texture *bar = loadBMPFromFile("/assets/bar_32x32.bmp", renderer);
    SDL_Texture *charset = loadBMPFromFile("/assets/cs8x8.bmp", renderer);

    // start game
    gameInit(state, renderer);
    gameLoop(state, renderer, head, body, tail, blueDot, redDot, border, bar, charset);

    // destroy textures, renderer and window after leaving the game
    SDL_DestroyTexture(head);
    SDL_DestroyTexture(body);
    SDL_DestroyTexture(tail);
    SDL_DestroyTexture(blueDot);
    SDL_DestroyTexture(redDot);
    SDL_DestroyTexture(bar);
    SDL_DestroyTexture(charset);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
