#include "./config.hpp"
#include <SDL2/SDL.h>
#include <string.h>
#pragma once

// to calculate rotation degree by snake's direction
int getRotationDeg(int dx, int dy) {
    if (dx == 1 && dy == 0) {
        return 90;
    } else if (dx == 0 && dy == 1) {
        return 180;
    } else if (dx == -1 && dy == 0) {
        return 270;
    }

    return 0;
}

// formating int value
void formatDigits(int value, char *formattedValue) {
    formattedValue[4] = '\0';
    for (int i = 3; i >= 0; i--) {
        formattedValue[i] = (value % 10) + '0';
        value /= 10;
    }
}

// loading bitmap into surface and creating texture
SDL_Texture *loadBMPFromFile(const char *path, SDL_Renderer *renderer) {

    SDL_Surface *surface = SDL_LoadBMP(path);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}

// drawing string on the screen
void drawString(SDL_Renderer *renderer, SDL_Texture *charset, const char *text, int length, int x, int y) {
    SDL_Rect srcRect, destRect;
    srcRect.w = 8;
    destRect.w = 16;
    srcRect.h = 8;
    destRect.h = 16;

    for (int i = 0; i < length; i++) {
        char character = text[i];

        int px = (character % 16) * 8;
        int py = (character / 16) * 8;

        srcRect.x = px;
        srcRect.y = py;
        destRect.x = x + (i)*destRect.w;
        destRect.y = y;

        SDL_RenderCopy(renderer, charset, &srcRect, &destRect);
    }
}

// drawing grid
void drawGrid(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0x18, 0x18, 0x18, 0xFF);
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            SDL_Rect cellRect = {x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
            SDL_RenderDrawRect(renderer, &cellRect);
        }
    }
}

// drawing snake
void drawSnake(SDL_Renderer *renderer, Snake &snake, SDL_Texture *head, SDL_Texture *body, SDL_Texture *tail) {
    SDL_Rect rect;
    int deg;

    // always draw snake's head
    rect = {snake.segments[0].x * CELL_SIZE, snake.segments[0].y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
    deg = getRotationDeg(snake.dx, snake.dy);

    SDL_RenderCopyEx(renderer, head, NULL, &rect, getRotationDeg(snake.dx, snake.dy), NULL, SDL_FLIP_NONE);

    // drawing snake's body if snake length is greater than 2
    if (snake.length > 2) {
        for (int i = 1; i < snake.length - 1; i++) {
            // getting body's directions
            int bodyDx = snake.segments[i - 1].x - snake.segments[i].x;
            int bodyDy = snake.segments[i - 1].y - snake.segments[i].y;

            rect = {snake.segments[i].x * CELL_SIZE, snake.segments[i].y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
            deg = getRotationDeg(bodyDx, bodyDy); // calculation of the degree of rotation of the snake depending on direction

            SDL_RenderCopyEx(renderer, body, NULL, &rect, deg, NULL, SDL_FLIP_NONE);
        }
    }

    // drawing snake's tail if length is greater than 1
    if (snake.length > 1) {
        // getting tail's directions
        int tailDx = snake.segments[snake.length - 2].x - snake.segments[snake.length - 1].x;
        int tailDy = snake.segments[snake.length - 2].y - snake.segments[snake.length - 1].y;

        rect = {snake.segments[snake.length - 1].x * CELL_SIZE, snake.segments[snake.length - 1].y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
        deg = getRotationDeg(tailDx, tailDy); // calculation of the degree of rotation of the snake depending on direction

        SDL_RenderCopyEx(renderer, tail, NULL, &rect, deg, NULL, SDL_FLIP_NONE);
    }
}

// drawing border around the board
void drawBorder(SDL_Renderer *renderer, SDL_Texture *border) {
    SDL_Rect borderRect;

    // top edge
    for (int i = 0; i < GRID_WIDTH; i++) {
        borderRect = {i * CELL_SIZE, 0, CELL_SIZE, CELL_SIZE};
        SDL_RenderCopy(renderer, border, NULL, &borderRect);
    }

    // bottom edge
    for (int i = 0; i < GRID_WIDTH; i++) {
        borderRect = {i * CELL_SIZE, (GRID_HEIGHT - 1) * CELL_SIZE, CELL_SIZE, CELL_SIZE};
        SDL_RenderCopy(renderer, border, NULL, &borderRect);
    }

    // left and right edge
    for (int i = 1; i < GRID_HEIGHT - 1; i++) {
        borderRect = {0, i * CELL_SIZE, CELL_SIZE, CELL_SIZE};
        SDL_RenderCopy(renderer, border, NULL, &borderRect);

        borderRect = {(GRID_WIDTH - 1) * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE};
        SDL_RenderCopy(renderer, border, NULL, &borderRect);
    }
}

// drawing food
void drawFood(SDL_Renderer *renderer, SDL_Texture *blueDot, Dot &food) {
    SDL_Rect foodRect = {food.position.x * CELL_SIZE, food.position.y * CELL_SIZE, CELL_SIZE, CELL_SIZE};

    SDL_RenderCopy(renderer, blueDot, NULL, &foodRect);
}

// drawing bonus
void drawBonus(SDL_Renderer *renderer, SDL_Texture *redDot, Dot *bonus) {
    if (bonus != NULL) { // checking if bonus is active
        SDL_Rect bonusRect = {bonus->position.x * CELL_SIZE, bonus->position.y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
        SDL_RenderCopy(renderer, redDot, NULL, &bonusRect);
    }
}

// drawing progressbar
void drawProgressBar(GameState &state, SDL_Renderer *renderer, SDL_Texture *bar, SDL_Texture *charset) {
    char formattedBonusTime[5];

    formatDigits(state.bonus_timer, formattedBonusTime);

    if (state.bonus != NULL) { // if bonus is active
        int activeSegments = state.bonus_timer / 2;

        const char text[] = "Bonus time left: ";
        drawString(renderer, charset, text, 17, BOARD_WIDTH / 2 - (8 * 16), BOARD_HEIGHT + 144);

        // drawing single bars
        for (int i = 0; i <= activeSegments; i++) {
            SDL_Rect barRect = {BOARD_WIDTH / 2 + (i * 32) - (9 * 16), BOARD_HEIGHT + 172, CELL_SIZE, CELL_SIZE};
            drawString(renderer, charset, formattedBonusTime, 4, BOARD_WIDTH / 2 + (9 * 16), BOARD_HEIGHT + 144);
            SDL_RenderCopy(renderer, bar, NULL, &barRect);
        }
    }
}

void drawEndScreen(GameState &state, SDL_Renderer *renderer, SDL_Texture *charset) {
    SDL_SetRenderDrawColor(renderer, 0x12, 0x12, 0x12, 0xFF);
    SDL_RenderClear(renderer);

    char formattedScore[5];

    formatDigits(state.score, formattedScore);

    const char text[] = "Score: ";
    drawString(renderer, charset, text, 8, BOARD_WIDTH / 2 - (5 * 16), BOARD_HEIGHT / 2);
    drawString(renderer, charset, formattedScore, 4, BOARD_WIDTH / 2 + (3 * 16), BOARD_HEIGHT / 2);

    const char text1[] = "Press 'esc' to quit ";
    drawString(renderer, charset, text1, 20, BOARD_WIDTH / 2 - (10 * 16), BOARD_HEIGHT / 2 + 64);
    const char text2[] = "or 'n' to start new game";
    drawString(renderer, charset, text2, 24, BOARD_WIDTH / 2 - (12 * 16), BOARD_HEIGHT / 2 + 100);

    SDL_RenderPresent(renderer);
}

void drawStatus(GameState &state, SDL_Renderer *renderer, SDL_Texture *charset) {

    char formattedTime[5];
    formatDigits(state.timer, formattedTime);
    char formattedScore[5];
    formatDigits(state.score, formattedScore);

    // draw done requirments
    const char text[] = "Requirments: 1, 2, 3, 4, A, B, C, D";
    drawString(renderer, charset, text, 35, BOARD_WIDTH / 2 - (18 * 16), BOARD_HEIGHT + 16);

    // draw elapsed time
    const char text1[] = "Time: ";
    drawString(renderer, charset, text1, 6, BOARD_WIDTH / 2 - (3 * 16), BOARD_HEIGHT + 36);
    drawString(renderer, charset, formattedTime, 4, BOARD_WIDTH / 2 - (2 * 16) + 72, BOARD_HEIGHT + 36);

    // draw score
    const char text2[] = "Score: ";
    drawString(renderer, charset, text2, 7, BOARD_WIDTH / 2 - (3 * 16), BOARD_HEIGHT + 56);
    drawString(renderer, charset, formattedScore, 4, BOARD_WIDTH / 2 - (2 * 16) + 80, BOARD_HEIGHT + 56);
}

void renderGame(GameState &state, SDL_Renderer *renderer, SDL_Texture *head, SDL_Texture *body,
                SDL_Texture *tail, SDL_Texture *blueDot, SDL_Texture *redDot, SDL_Texture *border,
                SDL_Texture *bar, SDL_Texture *charset) {

    SDL_SetRenderDrawColor(renderer, 0x12, 0x12, 0x12, 0xFF);
    SDL_RenderClear(renderer);

    // drawing all things
    drawBorder(renderer, border);
    drawGrid(renderer);
    drawFood(renderer, blueDot, state.food);
    drawBonus(renderer, redDot, state.bonus);
    drawSnake(renderer, state.snake, head, body, tail);
    drawStatus(state, renderer, charset);
    drawProgressBar(state, renderer, bar, charset);

    SDL_RenderPresent(renderer);
}