#include <stdio.h>
#include <time.h>

#include "controller.h"
#include "model.h"
#include "view.h"

int main(void) {
  srand(time(NULL));
  SDL_Renderer* renderer = NULL;

  int game_running = 0;
  int position[] = {3, 0};

  BoardCell board_state[NUM_ROWS][NUM_COLS];

  tetromino current_piece = {
      .rows = 4,
      .cols = 4,
      .shape = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
      .color = {.r = 255, .g = 255, .b = 255, .a = 255},
      .letter = 'A'};
  int rotation_state = 0;

  int dropped = 0;

  size_t level = 1;
  size_t score = 0;

  initialize_window(&renderer);
  renderStartScreen(renderer);
  // Game loop
  while (!game_running) {
    if (handle_mouse_click(renderer)) {
      game_running = 1;
      setup(board_state, &current_piece, position);
      score = 0;
      level = 1;
    }
  };

  while (game_running) {
    process_input(&game_running, position, board_state, &current_piece,
                  &dropped, &rotation_state, &score);
    if (!game_over(board_state)) {  // Check for game over condition
      render(renderer, position, board_state, &current_piece, &score, &level);
      update(position, board_state, &current_piece, &dropped, &rotation_state,
             &score, &level);
    } else {
      // Game over logic here (e.g., display game over message)
      renderGameOverScreen(renderer);
      if (handle_mouse_click(renderer)) {
        setup(board_state, &current_piece, position);
        score = 0;
        level = 1;
      };
    }
  }
  destroy_window(renderer);
  return 0;
}
