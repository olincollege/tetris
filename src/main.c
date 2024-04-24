
#include <stdio.h>
#include <time.h>

#include "controller.h"
#include "model.h"
#include "view.h"

int main(void) {
  srand(time(NULL));
  SDL_Renderer* renderer = NULL;

  int game_running = 0;
  float position[] = {3, 0};

  BoardCell board_state[20][10];

  tetromino current_piece = {
      .rows = 4,
      .cols = 4,
      .shape = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
      .color = {.r = 255, .g = 255, .b = 255, .a = 255},
      .letter = 'A'};
  int rotation_state = 0;

  int dropped = 0;

  size_t score = 0;

  game_running = initialize_window(&renderer);
  setup(board_state, &current_piece, position);

  while (game_running) {
    process_input(&game_running, position, board_state, &current_piece,
                  &dropped, &rotation_state, &score);
    if (!game_over(board_state)) {  // Check for game over condition
      render(renderer, position, board_state, &current_piece, &score);
      update(position, board_state, &current_piece, &dropped, &rotation_state,
             &score);
    } else {
      // Game over logic here (e.g., display game over message)
      printf("Game Over!\n");
      game_running = 0;
    }
  }

  destroy_window(renderer);

  return 0;
}
