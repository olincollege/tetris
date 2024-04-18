
#include <SDL2/SDL.h>

#include "controller.h"
#include "model.h"
#include "view.h"

int main(void) {
  int game_running = 0;
  SDL_Renderer* renderer = NULL;
  SDL_Window* window = NULL;
  game_running = initialize_window(renderer, window);

  int current_index = 0;
  int bag[] = {0, 1, 2, 3, 4, 5, 6};
  float position[] = {3, 0};

  tetromino tetrominos[7] = {// I
                             {.rows = 2,
                              .cols = 4,
                              .shape = {{0, 0, 0, 0}, {1, 1, 1, 1}},
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

  setup(bag, board_state);
  printf("here1\n");

  while (game_running) {
    printf("here\n");
    process_input(&game_running, position, &current_index, bag, tetrominos);
    if (!game_over()) {  // Check for game over condition
      render(position, &current_index, bag, tetrominos, board_state);
      update(position, &current_index, bag, tetrominos, board_state);
    } else {
      // Game over logic here (e.g., display game over message)
      printf("Game Over!\n");
      game_running = 0;
    }
  }

  destroy_window(renderer, window);

  return 0;
}
