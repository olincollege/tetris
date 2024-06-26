#include "view.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>  // For sprintf

#include "model.h"

void render(SDL_Renderer* renderer, const int* position,
            BoardCell (*board_state)[NUM_COLS], const size_t* score,
            tetromino* current_piece, const size_t* level) {
  SDL_SetRenderDrawColor(renderer, BACKGROUND_R, BACKGROUND_G, BACKGROUND_B,
                         BACKGROUND_A);
  SDL_RenderClear(renderer);

  for (int i = 0; i < NUM_COLS; i++) {
    for (int j = 0; j < NUM_ROWS; j++) {
      SDL_Rect shape_rect = {SQUARE_WIDTH + (i * SQUARE_WIDTH),
                             SQUARE_WIDTH + (j * SQUARE_WIDTH), SQUARE_WIDTH,
                             SQUARE_WIDTH};

      // adding grid lines
      if (board_state[j][i].filled == 1) {
        SDL_SetRenderDrawColor(
            renderer, board_state[j][i].color.r, board_state[j][i].color.g,
            board_state[j][i].color.b, board_state[j][i].color.a);
        SDL_RenderFillRect(renderer, &shape_rect);
      }
      SDL_SetRenderDrawColor(renderer, LINE_R, LINE_G, LINE_B, LINE_A);
      SDL_RenderDrawRect(renderer, &shape_rect);
    }
  }

  for (int i = 0; i < current_piece->cols; i++) {
    for (int j = 0; j < current_piece->rows; j++) {
      SDL_Rect shape_rect = {
          SQUARE_WIDTH + (position[0] * SQUARE_WIDTH + i * SQUARE_WIDTH),
          SQUARE_WIDTH + (position[1] * SQUARE_WIDTH + j * SQUARE_WIDTH),
          SQUARE_WIDTH, SQUARE_WIDTH};
      if (current_piece->shape[j][i] == 1) {
        SDL_SetRenderDrawColor(renderer, current_piece->color.r,
                               current_piece->color.g, current_piece->color.b,
                               current_piece->color.a);
        SDL_RenderFillRect(renderer, &shape_rect);
      }
      SDL_SetRenderDrawColor(renderer, LINE_R, LINE_G, LINE_B, LINE_A);
      SDL_RenderDrawRect(renderer, &shape_rect);
    }
  }

  SDL_Rect shape_rect_level = {LEVEL_POS_X * SQUARE_WIDTH,
                               LEVEL_POS_Y * SQUARE_WIDTH, 4 * SQUARE_WIDTH,
                               1 * SQUARE_WIDTH};
  SDL_Rect shape_rect_score = {SCORE_POS_X * SQUARE_WIDTH,
                               SCORE_POS_Y * SQUARE_WIDTH, 4 * SQUARE_WIDTH,
                               2 * SQUARE_WIDTH};
  SDL_SetRenderDrawColor(renderer, LINE_R, LINE_G, LINE_B, LINE_A);
  SDL_RenderDrawRect(renderer, &shape_rect_level);
  SDL_RenderDrawRect(renderer, &shape_rect_score);
  // Initialize SDL_ttf
  TTF_Init();

  // declare a TTF_Font
  TTF_Font* font = NULL;
  int renderstyle = 0;
  renderstyle = TTF_STYLE_NORMAL;

  font = TTF_OpenFont("./src/DejaVuSansMono.ttf", FONT_SIZE_SCORE);
  if (font == NULL) {
    (void)fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
  }
  TTF_SetFontStyle(font, renderstyle);

  // Render score text
  SDL_Color textColor = {TEXT_R, TEXT_G, TEXT_B, TEXT_A};
  char levelText[STRING_BUFFER_LEN];
  char scoreText[STRING_BUFFER_LEN];

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

void renderStartScreen(SDL_Renderer* renderer) {
  // Clear the screen
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, MAX_COLOR_VAL);
  SDL_RenderClear(renderer);

  // Render "Game Over" text with rainbow colors
  TTF_Init();
  TTF_Font* font = TTF_OpenFont("./src/DejaVuSansMono.ttf", FONT_SIZE_TETRIS);
  if (font == NULL) {
    (void)fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
    return;
  }

  SDL_Rect textRect;
  const SDL_Color rainbowColors[] = {
      {255, 0, 0, 255},    // Red
      {255, 165, 0, 255},  // Orange
      {255, 255, 0, 255},  // Yellow
      {0, 128, 0, 255},    // Green
      {0, 0, 255, 255},    // Blue
      {75, 0, 130, 255},   // Indigo
      {128, 0, 128, 255}   // Violet
  };

  char* startText = "TETRIS";
  int x_game_start =
      (SCREEN_WIDTH - TETRIS_WIDTH) / 2;  // Center the text horizontally
  int y_game_start = SCREEN_HEIGHT / 2;   // Center the text vertically
  SDL_RenderClear(renderer);

  for (size_t i = 0; i < strlen(startText); i++) {
    char* currentChar = startText[i];
    SDL_Surface* letterSurface = TTF_RenderText_Solid(
        font, &currentChar, rainbowColors[i % NUM_TETROMINOS]);
    SDL_Texture* letterTexture =
        SDL_CreateTextureFromSurface(renderer, letterSurface);

    SDL_QueryTexture(letterTexture, NULL, NULL, &textRect.w, &textRect.h);
    textRect.x = x_game_start + (int)i * TETRIS_LETTER_WIDTH;
    textRect.y = y_game_start;

    SDL_RenderCopy(renderer, letterTexture, NULL, &textRect);

    SDL_FreeSurface(letterSurface);
    SDL_DestroyTexture(letterTexture);
  }

  // Render "Start Game" button
  TTF_Font* buttonFont =
      TTF_OpenFont("./src/DejaVuSansMono.ttf", FONT_SIZE_BUTTON);
  if (buttonFont == NULL) {
    (void)fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
    return;
  }
  SDL_Color textColor = {0, 0, 0, MAX_COLOR_VAL};
  SDL_Surface* buttonSurface =
      TTF_RenderText_Solid(buttonFont, "Start Game", textColor);
  SDL_Texture* buttonTexture =
      SDL_CreateTextureFromSurface(renderer, buttonSurface);

  SDL_QueryTexture(buttonTexture, NULL, NULL, &textRect.w, &textRect.h);
  textRect.x = (SCREEN_WIDTH - BUTTON_WIDTH) / 2;
  textRect.y = y_game_start + (BUTTON_HEIGHT / 2);

  // Render "Start Over" button background
  SDL_Rect buttonBackgroundRect = {textRect.x - 10, textRect.y - 5,
                                   textRect.w + 20, textRect.h + 10};
  SDL_SetRenderDrawColor(renderer, MAX_COLOR_VAL, MAX_COLOR_VAL, MAX_COLOR_VAL,
                         MAX_COLOR_VAL);
  SDL_RenderFillRect(renderer, &buttonBackgroundRect);

  // Render "Start Over" button border
  SDL_SetRenderDrawColor(renderer, 0, 0, 0,
                         MAX_COLOR_VAL);  // Black border color
  SDL_RenderDrawRect(renderer, &buttonBackgroundRect);

  // Render "Start Over" button text
  SDL_RenderCopy(renderer, buttonTexture, NULL, &textRect);

  // Cleanup
  SDL_FreeSurface(buttonSurface);
  SDL_DestroyTexture(buttonTexture);
  TTF_CloseFont(font);
  TTF_CloseFont(buttonFont);

  SDL_RenderPresent(renderer);
}

void renderGameOverScreen(SDL_Renderer* renderer) {
  // Clear the screen
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, MAX_COLOR_VAL);
  SDL_RenderClear(renderer);

  // Render "Game Over" text with rainbow colors
  TTF_Init();
  TTF_Font* font = TTF_OpenFont("./src/DejaVuSansMono.ttf", FONT_SIZE_TETRIS);
  if (font == NULL) {
    (void)fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
    return;
  }

  SDL_Rect textRect;
  const SDL_Color rainbowColors[] = {
      {255, 0, 0, 255},    // Red
      {255, 165, 0, 255},  // Orange
      {255, 255, 0, 255},  // Yellow
      {0, 128, 0, 255},    // Green
      {0, 0, 255, 255},    // Blue
      {75, 0, 130, 255},   // Indigo
      {128, 0, 128, 255}   // Violet
  };

  char* gameOverText = "GameOver";
  int x_game_over =
      (SCREEN_WIDTH - GAMEOVER_WIDTH) / 2;  // Center the text horizontally
  int y_game_over = SCREEN_HEIGHT / 2;      // Center the text vertically
  SDL_RenderClear(renderer);

  for (size_t i = 0; i < strlen(gameOverText); i++) {
    char* currentChar = gameOverText[i];
    SDL_Surface* letterSurface = TTF_RenderText_Solid(
        font, &currentChar, rainbowColors[i % NUM_TETROMINOS]);
    SDL_Texture* letterTexture =
        SDL_CreateTextureFromSurface(renderer, letterSurface);

    SDL_QueryTexture(letterTexture, NULL, NULL, &textRect.w, &textRect.h);
    textRect.x = x_game_over + (int)i * TETRIS_LETTER_WIDTH;
    textRect.y = y_game_over;

    SDL_RenderCopy(renderer, letterTexture, NULL, &textRect);

    SDL_FreeSurface(letterSurface);
    SDL_DestroyTexture(letterTexture);
  }

  // Render "Start Over" button
  TTF_Font* buttonFont =
      TTF_OpenFont("./src/DejaVuSansMono.ttf", FONT_SIZE_BUTTON);
  if (buttonFont == NULL) {
    (void)fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
    return;
  }
  SDL_Color textColor = {0, 0, 0, MAX_COLOR_VAL};
  SDL_Surface* buttonSurface =
      TTF_RenderText_Solid(buttonFont, "Start Over", textColor);
  SDL_Texture* buttonTexture =
      SDL_CreateTextureFromSurface(renderer, buttonSurface);

  SDL_QueryTexture(buttonTexture, NULL, NULL, &textRect.w, &textRect.h);
  textRect.x = (SCREEN_WIDTH - BUTTON_WIDTH) / 2;
  textRect.y = y_game_over + (BUTTON_HEIGHT / 2);

  // Render "Start Over" button background
  SDL_Rect buttonBackgroundRect = {textRect.x - 10, textRect.y - 5,
                                   textRect.w + 20, textRect.h + 10};
  SDL_SetRenderDrawColor(renderer, MAX_COLOR_VAL, MAX_COLOR_VAL, MAX_COLOR_VAL,
                         MAX_COLOR_VAL);  // White background color
  SDL_RenderFillRect(renderer, &buttonBackgroundRect);

  // Render "Start Over" button border
  SDL_SetRenderDrawColor(renderer, 0, 0, 0,
                         MAX_COLOR_VAL);  // Black border color
  SDL_RenderDrawRect(renderer, &buttonBackgroundRect);

  // Render "Start Over" button text
  SDL_RenderCopy(renderer, buttonTexture, NULL, &textRect);

  // Cleanup
  SDL_FreeSurface(buttonSurface);
  SDL_DestroyTexture(buttonTexture);
  TTF_CloseFont(font);
  TTF_CloseFont(buttonFont);

  SDL_RenderPresent(renderer);
}
