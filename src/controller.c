#include <SDL2/SDL.h>

#include "model.h"

void process_input(int* game_running, float* position,
                   BoardCell (*board_state)[10], tetromino* current_piece,
                   int* dropped, int* rotation_state) {
  SDL_Event event;
  SDL_PollEvent(&event);

  direction dir_in_place = {.horizontal = 0, .vertical = 0};
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
          break;
        }
      } else if (event.key.keysym.sym == SDLK_SPACE) {
        // Hard drop
        while (
            !check_collisions(position, board_state, current_piece, dir_down)) {
          position[1] += 1;
          if (position[1] + current_piece->rows > 20) {
            position[1] = 20 - current_piece->rows;
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
