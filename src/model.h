#pragma once

#include <SDL2/SDL.h>

enum {
  SQUARE_WIDTH = 40,
};

typedef struct {
  int filled;
  SDL_Color color;
} BoardCell;

typedef struct tetromino {
  int rows;
  int cols;
  int shape[4][4];
  SDL_Color color;
} tetromino;
