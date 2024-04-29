#include "view.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>  // For sprintf

#include "model.h"

void render(SDL_Renderer* renderer, float* position,
            BoardCell (*board_state)[10], tetromino* current_piece,
            size_t* score, size_t* level) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

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

  for (int i = 0; i < current_piece->cols; i++) {
    for (int j = 0; j < current_piece->rows; j++) {
      SDL_Rect shape_rect = {
          40 + (int)(position[0] * SQUARE_WIDTH + i * SQUARE_WIDTH),
          40 + (int)(position[1] * SQUARE_WIDTH + j * SQUARE_WIDTH),
          SQUARE_WIDTH, SQUARE_WIDTH};
      if (current_piece->shape[j][i] == 1) {
        SDL_SetRenderDrawColor(renderer, current_piece->color.r,
                               current_piece->color.g, current_piece->color.b,
                               current_piece->color.a);
        SDL_RenderFillRect(renderer, &shape_rect);
      }
      SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
      SDL_RenderDrawRect(renderer, &shape_rect);
    }
  }

  SDL_Rect shape_rect_level = {12 * SQUARE_WIDTH, 1 * SQUARE_WIDTH,
                               4 * SQUARE_WIDTH, 1 * SQUARE_WIDTH};
  SDL_Rect shape_rect_score = {12 * SQUARE_WIDTH, 2 * SQUARE_WIDTH,
                               4 * SQUARE_WIDTH, 2 * SQUARE_WIDTH};
  SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
  SDL_RenderDrawRect(renderer, &shape_rect_level);
  SDL_RenderDrawRect(renderer, &shape_rect_score);
  // Initialize SDL_ttf
  TTF_Init();

  // declare a TTF_Font
  TTF_Font* font;
  int renderstyle;
  renderstyle = TTF_STYLE_NORMAL;

  font = TTF_OpenFont("./src/DejaVuSansMono.ttf", 12);
  if (font == NULL) {
    fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
  }
  TTF_SetFontStyle(font, renderstyle);

  // Render score text
  SDL_Color textColor = {255, 255, 255, 255};
  char levelText[50];
  char scoreText[50];

  sprintf(levelText, "Level: %zu", *level);
  sprintf(scoreText, "Score: %zu", *score);

  SDL_Surface* levelSurface = TTF_RenderText_Solid(font, levelText, textColor);
  SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText, textColor);

  SDL_Texture* levelTexture =
      SDL_CreateTextureFromSurface(renderer, levelSurface);
  SDL_Texture* scoreTexture =
      SDL_CreateTextureFromSurface(renderer, scoreSurface);

  SDL_FreeSurface(levelSurface);
  SDL_FreeSurface(scoreSurface);

  SDL_RenderCopy(renderer, levelTexture, NULL, &shape_rect_level);
  SDL_RenderCopy(renderer, scoreTexture, NULL, &shape_rect_score);

  SDL_DestroyTexture(levelTexture);
  SDL_DestroyTexture(scoreTexture);

  TTF_CloseFont(font);

  SDL_RenderPresent(renderer);
}
