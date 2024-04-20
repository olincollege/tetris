
#include <stdio.h>

#include "controller.h"
#include "model.h"
#include "view.h"

int main(void) {
  SDL_Renderer* renderer = NULL;

  int game_running = 0;
  float position[] = {3, 0};
  int current_index = 0;
  int bag[] = {0, 1, 2, 3, 4, 5, 6};

  tetromino tetrominos[7] = {// I
                             {.rows = 1,
                              .cols = 4,
                              .shape = {{1, 1, 1, 1}},
                              .color = {.r = 0, .g = 255, .b = 255, .a = 255}},
                             // J
                             {.rows = 2,
                              .cols = 3,
                              .shape = {{1, 0, 0}, {1, 1, 1}},
                              .color = {.r = 0, .g = 0, .b = 255, .a = 255}},
                             // L
                             {.rows = 2,
                              .cols = 3,
                              .shape = {{0, 0, 1}, {1, 1, 1}},
                              .color = {.r = 255, .g = 165, .b = 0, .a = 255}},
                             // O
                             {.rows = 2,
                              .cols = 2,
                              .shape = {{1, 1}, {1, 1}},
                              .color = {.r = 255, .g = 255, .b = 0, .a = 255}},
                             // S
                             {.rows = 2,
                              .cols = 3,
                              .shape = {{0, 1, 1}, {1, 1, 0}},
                              .color = {.r = 0, .g = 255, .b = 0, .a = 255}},
                             // T
                             {.rows = 2,
                              .cols = 3,
                              .shape = {{0, 1, 0}, {1, 1, 1}},
                              .color = {.r = 128, .g = 0, .b = 255, .a = 255}},
                             // Z
                             {.rows = 2,
                              .cols = 3,
                              .shape = {{1, 1, 0}, {0, 1, 1}},
                              .color = {.r = 255, .g = 0, .b = 0, .a = 255}}};

  BoardCell board_state[20][10];

  tetromino current_piece = {
      .rows = 4,
      .cols = 4,
      .shape = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
      .color = {.r = 255, .g = 255, .b = 255, .a = 255}};

  int dropped = 0;

  game_running = initialize_window(&renderer);
  setup(bag, board_state, &current_piece, &current_index, tetrominos, position);

  while (game_running) {
    process_input(&game_running, position, board_state, &current_piece,
                  &dropped);
    if (!game_over(board_state)) {  // Check for game over condition
      render(renderer, position, board_state, &current_piece);
      update(position, &current_index, bag, board_state, &current_piece,
             tetrominos, &dropped);
    } else {
      // Game over logic here (e.g., display game over message)
      printf("Game Over!\n");
      game_running = 0;
    }
  }

  destroy_window(renderer);

  return 0;
}
