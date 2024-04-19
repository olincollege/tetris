#pragma once
#include <SDL2/SDL.h>

#include "model.h"

/***/
void render(SDL_Renderer* renderer, tetromino* tetrominos, int* bag,
            int* current_index, float* position, BoardCell (*board_state)[10]);
