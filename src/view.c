#include "view.h"

#include <SDL2/SDL.h>

#include "model.h"

void render(SDL_Renderer* renderer, tetromino* tetrominos, int* bag,
            int* current_index, float* position, BoardCell (*board_state)[10]) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  const tetromino shape = tetrominos[bag[*current_index]];

  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 20; j++) {
      SDL_Rect shape_rect = {40 + (int)(i * SQUARE_WIDTH),
                             40 + (int)(j * SQUARE_WIDTH), SQUARE_WIDTH,
                             SQUARE_WIDTH};

      // adding grid lines
      if (board_state[j][i].filled == 1) {
        SDL_SetRenderDrawColor(
            renderer, board_state[j][i].color.r, board_state[j][i].color.g,
            board_state[j][i].color.b, board_state[j][i].color.a);
        SDL_RenderFillRect(renderer, &shape_rect);
      }
      SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
      SDL_RenderDrawRect(renderer, &shape_rect);
    }
  }

  for (int i = 0; i < shape.cols; i++) {
    for (int j = 0; j < shape.rows; j++) {
      SDL_Rect shape_rect = {
          40 + (int)(position[0] * SQUARE_WIDTH + i * SQUARE_WIDTH),
          40 + (int)(position[1] * SQUARE_WIDTH + j * SQUARE_WIDTH),
          SQUARE_WIDTH, SQUARE_WIDTH};
      if (shape.shape[j][i] == 1) {
        SDL_SetRenderDrawColor(renderer, shape.color.r, shape.color.g,
                               shape.color.b, shape.color.a);
        SDL_RenderFillRect(renderer, &shape_rect);
      }
      SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
      SDL_RenderDrawRect(renderer, &shape_rect);
    }
  }

  SDL_Rect shape_rect = {12 * SQUARE_WIDTH, 2 * SQUARE_WIDTH, 4 * SQUARE_WIDTH,
                         3 * SQUARE_WIDTH};
  SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
  SDL_RenderDrawRect(renderer, &shape_rect);

  SDL_RenderPresent(renderer);
}