
#include "model.h"

#include <SDL2/SDL.h>

SDL_Window* window = NULL;

int last_frame_time = 0;

void shuffle_bag(int* array, size_t n) {
  if (n > 1) {
    size_t i;
    for (i = 0; i < n - 1; i++) {
      size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
      int t = array[j];
      array[j] = array[i];
      array[i] = t;
    }
  }
}

int initialize_window(SDL_Renderer** renderer) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL.\n");
    return 0;
  }

  window =
      SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       17 * SQUARE_WIDTH, 22 * SQUARE_WIDTH, 0);
  if (!window) {
    fprintf(stderr, "Error creating SDL Window.\n");
    return 0;
  }

  *renderer = SDL_CreateRenderer(window, -1, 0);
  if (!*renderer) {
    fprintf(stderr, "Error creating SDL Renderer.\n");
    return 0;
  }

  return 1;
}

void setup(int* bag, BoardCell (*board_state)[10], tetromino* current_piece,
           int* current_index, tetromino* tetrominos, float* position) {
  shuffle_bag(bag, 7);
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 10; j++) {
      board_state[i][j].filled = 0;
    }
  }
  set_current_piece(current_piece, current_index, bag, tetrominos, position);
}

void copy_shape_matrix(int dest[4][4], int template[4][4], int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      dest[i][j] = template[i][j];
    }
  }
}

void set_current_piece(tetromino* current_piece, int* current_index, int* bag,
                       tetromino* tetrominos, float* position) {
  // if I piece, spawn one space lower
  if (bag[*current_index] == 0) {
    position[1] = 1;
  }
  tetromino template = tetrominos[bag[*current_index]];
  current_piece->rows = template.rows;
  current_piece->cols = template.cols;
  current_piece->color = template.color;
  copy_shape_matrix(current_piece->shape, template.shape, template.rows,
                    template.cols);
}

void rotate_shape(float* position, BoardCell (*board_state)[10],
                  tetromino* current_piece, int clockwise) {
  int temp_shape[4][4];

  for (int i = 0; i < current_piece->rows; i++) {
    for (int j = 0; j < current_piece->cols; j++) {
      // left (counterclockwise)
      if (clockwise == 0) {
        temp_shape[current_piece->cols - j - 1][i] = current_piece->shape[i][j];
      } else {
        temp_shape[j][current_piece->rows - i - 1] = current_piece->shape[i][j];
      }
    }
  }

  tetromino temp_tetromino = {.rows = current_piece->cols,
                              .cols = current_piece->rows,
                              .color = current_piece->color};
  copy_shape_matrix(temp_tetromino.shape, temp_shape, current_piece->cols,
                    current_piece->rows);

  direction dir_in_place = {.horizontal = 0, .vertical = 0};
  if (!check_collisions(position, board_state, &temp_tetromino, dir_in_place)) {
    current_piece->cols = temp_tetromino.cols;
    current_piece->rows = temp_tetromino.rows;
    copy_shape_matrix(current_piece->shape, temp_tetromino.shape,
                      temp_tetromino.rows, temp_tetromino.cols);

    if ((int)position[0] + current_piece->cols > 10) {
      position[0] = 10 - current_piece->cols;
    }
  }
}

int check_collisions(float* position, BoardCell (*board_state)[10],
                     tetromino* current_piece, direction dir) {
  for (int i = 0; i < current_piece->cols; i++) {
    for (int j = 0; j < current_piece->rows; j++) {
      if (current_piece->shape[j][i] == 1) {
        if (board_state[(int)position[1] + j + dir.vertical]
                       [(int)position[0] + i + dir.horizontal]
                           .filled != 0) {
          return 1;  // Collision with another tetromino
        }
        if ((int)position[1] + j + 1 >= 20) {
          return 1;  // Reached the bottom of the board
        }
      }
    }
  }
  return 0;  // No collisions
}

int check_completed_lines(BoardCell (*board_state)[10]) {
  int lines_cleared = 0;

  for (int i = 0; i < 20; i++) {
    int line_complete = 1;
    for (int j = 0; j < 10; j++) {
      if (board_state[i][j].filled == 0) {
        line_complete = 0;
        break;
      }
    }

    if (line_complete) {
      for (int j = 0; j < 10; j++) {
        board_state[i][j].filled = 0;
      }

      for (int k = i; k > 0; k--) {
        for (int j = 0; j < 10; j++) {
          board_state[k][j].filled = board_state[k - 1][j].filled;
          board_state[k][j].color = board_state[k - 1][j].color;
        }
      }

      lines_cleared++;
    }
  }

  return lines_cleared;
}

int game_over(BoardCell (*board_state)[10]) {
  int game_over = 0;

  // Check each cell in the specified line
  for (int i = 0; i < 10; i++) {
    if (board_state[0][i].filled == 1) {
      game_over = 1;
      break;  // If any cell is filled, break the loop
    }
  }

  return game_over;
}

void update_board(float* position, BoardCell (*board_state)[10],
                  tetromino* current_piece) {
  for (int i = 0; i < current_piece->cols; i++) {
    for (int j = 0; j < current_piece->rows; j++) {
      if (current_piece->shape[j][i] == 1) {
        board_state[(int)position[1] + j][(int)position[0] + i].filled = 1;
        board_state[(int)position[1] + j][(int)position[0] + i].color =
            current_piece->color;
      }
    }
  }

  int lines_cleared = check_completed_lines(board_state);
  if (lines_cleared > 0) {
    // add score stuff here
  }
}

void update(float* position, int* current_index, int* bag,
            BoardCell (*board_state)[10], tetromino* current_piece,
            tetromino* tetrominos) {
  // delta time, converted to seconds
  float delta_time = (SDL_GetTicks() - last_frame_time) / (float)1000;

  if (delta_time >= 1) {
    last_frame_time = SDL_GetTicks();

    const int blocks_per_sec = 1;

    direction dir_down = {.horizontal = 0, .vertical = 1};
    int collision =
        check_collisions(position, board_state, current_piece, dir_down);

    if (collision) {
      update_board(position, board_state, current_piece);
      position[0] = 3;
      position[1] = 0;
      (*current_index)++;
      if (*current_index >= 7) {
        shuffle_bag(bag, 7);
        *current_index = 0;
      }
      set_current_piece(current_piece, current_index, bag, tetrominos,
                        position);
    } else {
      position[1] += blocks_per_sec * delta_time;
    }
  }
}

void destroy_window(SDL_Renderer* renderer) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
