#include "model.h"

#include <SDL2/SDL.h>

int last_frame_time = 0;

void shuffle_bag(int* array, size_t n) {
  if (n > 1) {
    size_t i;
    for (i = 0; i < n - 1; i++) {
      printf("%zu\n", i);
      size_t j = i + rand() / (RAND_MAX / (n - i) + 1);

      int t = array[j];
      array[j] = array[i];
      array[i] = t;
      printf("%zu\n", i);
    }
  }
}

int initialize_window(SDL_Renderer* renderer, SDL_Window* window) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL.\n");
    return 0;
  }

  window =
      SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       10 * SQUARE_WIDTH, 20 * SQUARE_WIDTH, 0);
  if (!window) {
    fprintf(stderr, "Error creating SDL Window.\n");
    return 0;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    fprintf(stderr, "Error creating SDL Renderer.\n");
    return 0;
  }

  return 1;
}

void setup(int* bag, BoardCell* board_state[10]) {
  printf("here3\n");
  shuffle_bag(bag, 7);
  printf("here4");
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 10; j++) {
      board_state[i][j].filled = 0;
    }
  }
}

int check_collisions(float* position, int* current_index, int* bag,
                     tetromino* tetrominos, BoardCell* board_state[10]) {
  const tetromino shape = tetrominos[bag[*current_index]];

  for (int i = 0; i < shape.cols; i++) {
    for (int j = 0; j < shape.rows; j++) {
      if (shape.shape[j][i] == 1) {
        if (board_state[(int)position[1] + j + 1][(int)position[0] + i]
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

int check_completed_lines(BoardCell* board_state[10]) {
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

int game_over(BoardCell* board_state[10]) {
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

void update_board(float* position, int* current_index, int* bag,
                  tetromino* tetrominos, BoardCell** board_state) {
  const tetromino shape = tetrominos[bag[*current_index]];

  for (int i = 0; i < shape.cols; i++) {
    for (int j = 0; j < shape.rows; j++) {
      if (shape.shape[j][i] == 1) {
        board_state[(int)position[1] + j][(int)position[0] + i].filled = 1;
        board_state[(int)position[1] + j][(int)position[0] + i].color =
            shape.color;
      }
    }
  }

  int lines_cleared = check_completed_lines(board_state);
  if (lines_cleared > 0) {
    // add score stuff here
  }
}

void update(float* position, int* current_index, int* bag,
            tetromino* tetrominos, BoardCell* board_state[10]) {
  // delta time, converted to seconds
  float delta_time = (SDL_GetTicks() - last_frame_time) / (float)1000;

  if (delta_time >= 1) {
    last_frame_time = SDL_GetTicks();

    const int blocks_per_sec = 1;

    int collision =
        check_collisions(position, current_index, bag, tetrominos, board_state);

    if (collision) {
      update_board(position, current_index, bag, tetrominos, board_state);
      position[0] = 3;
      position[1] = 0;
      *current_index++;
      if (*current_index >= 7) {
        shuffle_bag(bag, 7);
        *current_index = 0;
      }
      position[1] = 0;
    } else {
      position[1] += blocks_per_sec * delta_time;
    }
  }
}

void destroy_window(SDL_Renderer* renderer, SDL_Window* window) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
