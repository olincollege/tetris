#pragma once
#include <SDL2/SDL.h>

#include "model.h"

/***/
void render(SDL_Renderer* renderer, float* position,
            BoardCell (*board_state)[10], tetromino* current_piece,
            size_t* score);
