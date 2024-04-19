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

int initialize_window(SDL_Renderer** renderer);

void setup(int* bag, BoardCell (*board_state)[10]);

int check_collisions(tetromino* tetrominos, int* bag, int* current_index,
                     float* position, BoardCell (*board_state)[10]);

int check_completed_lines(BoardCell (*board_state)[10]);

int game_over(BoardCell (*board_state)[10]);

void update_board(tetromino* tetrominos, int* bag, int* current_index,
                  float* position, BoardCell (*board_state)[10]);

void update(float* position, int* current_index, int* bag,
            tetromino* tetrominos, BoardCell (*board_state)[10]);

void destroy_window(SDL_Renderer* renderer);
