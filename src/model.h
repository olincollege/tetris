#pragma once
#include <SDL2/SDL.h>

enum {
  SQUARE_WIDTH = 40,
};

typedef struct {
  int horizontal;
  int vertical;
} direction;

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

void setup(int* bag, BoardCell (*board_state)[10], tetromino* current_piece,
           int* current_index, tetromino* tetrominos, float* position);

void copy_shape_matrix(int dest[4][4], int template[4][4], int rows, int cols);

void set_current_piece(tetromino* current_piece, int* current_index, int* bag,
                       tetromino* tetrominos, float* position);

void rotate_shape(float* position, BoardCell (*board_state)[10],
                  tetromino* current_piece, int clockwise);

int check_collisions(float* position, BoardCell (*board_state)[10],
                     tetromino* current_piece, direction dir);

int check_completed_lines(BoardCell (*board_state)[10]);

int game_over(BoardCell (*board_state)[10]);

void update_board(float* position, BoardCell (*board_state)[10],
                  tetromino* current_piece);

void update(float* position, int* current_index, int* bag,
            BoardCell (*board_state)[10], tetromino* current_piece,
            tetromino* tetrominos, int* dropped);

void destroy_window(SDL_Renderer* renderer);
