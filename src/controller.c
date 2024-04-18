#include "controller.h"

#include <SDL2/SDL.h>

#include "model.h"

void process_input(int* game_running, float* position, int* current_index,
                   int* bag, tetromino* tetrominos) {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
    case SDL_QUIT:
      *game_running = 0;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        *game_running = 0;
      } else if (event.key.keysym.sym == SDLK_LEFT) {
        position[0] -= 1;
        if (position[0] < 0) {
          position[0] = 0;
        }
      } else if (event.key.keysym.sym == SDLK_RIGHT) {
        position[0] += 1;
        if (position[0] + tetrominos[bag[*current_index]].cols > 10) {
          position[0] = 10 - tetrominos[bag[*current_index]].cols;
        }
      } else if (event.key.keysym.sym == SDLK_DOWN) {
        // Soft drop
        while (!check_collisions()) {
          position[1] += 1;
          break;
        }
      } else if (event.key.keysym.sym == SDLK_SPACE) {
        // Hard drop
        while (!check_collisions()) {
          position[1] += 1;
          if (position[1] + tetrominos[bag[*current_index]].rows > 20) {
            position[1] = 20 - tetrominos[bag[*current_index]].rows;
            break;
          }
        }
      }
      break;
  }
}
