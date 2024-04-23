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
  char letter;
} tetromino;

/**
 * Shuffle the bag of indices into the array of tetromino templates.
 *
 * The tetrominos are picked in the order of their indices in the bag. Once all
 * 7 pieces are picked, the bag is reshuffled.
 *
 * @param array The array of indices.
 * @param n The length of the bag array.
 */
void shuffle_bag(int* array, size_t n);

/***/
int initialize_window(SDL_Renderer** renderer);

/***/
void setup(BoardCell (*board_state)[10], tetromino* current_piece,
           float* position);

/***/
void copy_shape_matrix(int dest[4][4], int template[4][4], int rows, int cols);

/***/
void set_current_piece(tetromino* current_piece, float* position);

/***/
void rotate_shape(float* position, BoardCell (*board_state)[10],
                  tetromino* current_piece, int clockwise, int* rotation_state);

/***/
int available_position(float* temp_position, BoardCell (*board_state)[10],
                       tetromino* temp_tetromino);

/***/
int check_collisions(float* position, BoardCell (*board_state)[10],
                     tetromino* current_piece, direction dir);

/***/
int check_completed_lines(BoardCell (*board_state)[10], size_t* score);

/***/
int game_over(BoardCell (*board_state)[10]);

/***/
void update_board(float* position, BoardCell (*board_state)[10],
                  tetromino* current_piece, size_t* score);

/***/
float get_time_int(void);

/***/
void update(float* position, BoardCell (*board_state)[10],
            tetromino* current_piece, int* dropped, int* rotation_state,
            size_t* score);

/***/
void destroy_window(SDL_Renderer* renderer);
