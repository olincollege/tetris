#pragma once
#include <SDL2/SDL.h>

#include "model.h"

/**
 * Draw the contents of the board state to the SDL window.
 *
 * @param renderer The 2D rendering context for the SDL window.
 * @param position An array holding the x, y position of the piece.
 * @param board_state An array of pointers to arrays holding the contents of
 * each board row.
 * @param current_piece The currently falling tetromino struct.
 * @param score The current score.
 * @param level The current level.
 */
void render(SDL_Renderer* renderer, const int* position,
            BoardCell (*board_state)[NUM_COLS], tetromino* current_piece,
            const size_t* score, const size_t* level);
