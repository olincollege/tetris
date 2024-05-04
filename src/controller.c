#include "controller.h"

#include <SDL2/SDL.h>

#include "model.h"

void process_input(int* game_running, BoardCell (*board_state)[NUM_COLS],
                   int* position, tetromino* current_piece, int* dropped,
                   size_t* score, int* rotation_state) {
  SDL_Event event;
  SDL_PollEvent(&event);

  direction dir_left = {.horizontal = -1, .vertical = 0};
  direction dir_right = {.horizontal = 1, .vertical = 0};
  direction dir_down = {.horizontal = 0, .vertical = 1};
  switch (event.type) {
    case SDL_QUIT:
      *game_running = 0;
      break;
    case SDL_KEYDOWN:

      if (event.key.keysym.sym == SDLK_ESCAPE) {
        *game_running = 0;
      } else if (event.key.keysym.sym == SDLK_LEFT) {
        if (!check_collisions(position, board_state, current_piece, dir_left)) {
          position[0] -= 1;
        }
      } else if (event.key.keysym.sym == SDLK_RIGHT) {
        if (!check_collisions(position, board_state, current_piece,
                              dir_right)) {
          position[0] += 1;
        }
      } else if (event.key.keysym.sym == SDLK_DOWN) {
        // Soft drop
        while (
            !check_collisions(position, board_state, current_piece, dir_down)) {
          position[1] += 1;
          *score += 1;
          break;
        }
      } else if (event.key.keysym.sym == SDLK_SPACE) {
        // Hard drop
        while (
            !check_collisions(position, board_state, current_piece, dir_down)) {
          position[1] += 1;
          *score += 2;
          if (position[1] + current_piece->rows > NUM_ROWS) {
            position[1] = NUM_ROWS - current_piece->rows;
            break;
          }
        }
        *dropped = 1;
      } else if (event.key.keysym.sym == SDLK_z) {
        rotate_shape(position, board_state, current_piece, 0, rotation_state);
      } else if (event.key.keysym.sym == SDLK_UP) {
        rotate_shape(position, board_state, current_piece, 1, rotation_state);
      }
      break;
  }
}

int handle_mouse_click(SDL_Renderer* renderer) {
  int reset_game = 0;
  int buttonLeft = (SCREEN_WIDTH - BUTTON_WIDTH) / 2;
  int buttonRight = buttonLeft + BUTTON_WIDTH;
  int buttonTop = SCREEN_HEIGHT / 2 + (BUTTON_HEIGHT / 2);
  int buttonBottom = buttonTop + BUTTON_HEIGHT;

  SDL_Event event;  // Initialize event
  SDL_PollEvent(&event);

  // Check for events
  switch (event.type) {
    case SDL_QUIT:
      destroy_window(renderer);
      exit(EXIT_SUCCESS);  // Exit the program directly

      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        destroy_window(renderer);
        exit(EXIT_SUCCESS);  // Exit the program directly
      }
      break;
    case SDL_MOUSEBUTTONDOWN:
      // Check if the mouse button down event occurred within the "Start
      // Over" button boundaries
      if (event.button.button == SDL_BUTTON_LEFT &&
          event.button.x >= buttonLeft && event.button.x <= buttonRight &&
          event.button.y >= buttonTop && event.button.y <= buttonBottom) {
        reset_game = 1;
      }
      break;
  }
  return reset_game;
}
