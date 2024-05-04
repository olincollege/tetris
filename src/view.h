#pragma once
#include <SDL2/SDL.h>

#include "model.h"

enum {
  BACKGROUND_R = 0,
  BACKGROUND_G = 0,
  BACKGROUND_B = 0,
  BACKGROUND_A = 255,
  LINE_R = 100,
  LINE_G = 100,
  LINE_B = 100,
  LINE_A = 255,
  TEXT_R = 255,
  TEXT_G = 255,
  TEXT_B = 255,
  TEXT_A = 255,
  FONT_SIZE_SCORE = 12,
  FONT_SIZE_TETRIS = 36,
  FONT_SIZE_BUTTON = 24,
  STRING_BUFFER_LEN = 50,
  TETRIS_LETTER_WIDTH = 36,
  TETRIS_WIDTH = 360,
  GAMEOVER_WIDTH = 440,
};

/**
 * Draw the contents of the board state to the SDL window.
 *
 * @param renderer The 2D rendering context for the SDL window.
 * @param position An array holding the x, y position of the piece.
 * @param board_state An array of pointers to arrays holding the contents of
 * each board row.
 * @param score The current score.
 * @param current_piece The currently falling tetromino struct.
 * @param level The current level.
 */
void render(SDL_Renderer* renderer, const int* position,
            BoardCell (*board_state)[NUM_COLS], const size_t* score,
            tetromino* current_piece, const size_t* level);

void renderStartScreen(SDL_Renderer* renderer);

void renderGameOverScreen(SDL_Renderer* renderer);
