#include <SDL2/SDL.h>
#include <ctime>

#include "config.hpp"
#include "game_logic.hpp"

int main() {
    srand(time(0));

    GameState state;

    game(state);

    return 0;
};
