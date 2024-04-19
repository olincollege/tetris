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

void shuffle_bag(int* array, size_t n);

int initialize_window(void);

void setup(void);

int check_collisions(void);

int check_completed_lines(void);

int game_over(void);

void update_board(void);

void update(void);

void destroy_window(void);
