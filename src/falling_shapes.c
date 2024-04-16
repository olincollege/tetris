#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

int game_running = 0;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int last_frame_time = 0;

const int SQUARE_WIDTH = 40;
float position[] = {3, 0};
int current_index = 0;
int bag[] = {0, 1, 2, 3, 4, 5, 6};

typedef struct {
  int filled;
  SDL_Color color;
} BoardCell;

typedef struct tetromino {
  int rows;
  int cols;
  int shape[4][4];
  SDL_Color color;
} tetromino;

tetromino tetrominos[7] = {
    // I
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

void shuffle_bag(int* array, size_t n) {
  if (n > 1) {
    size_t i;
    for (i = 0; i < n - 1; i++) {
      size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
      int t = array[j];
      array[j] = array[i];
      array[i] = t;
    }
  }
}

int initialize_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL.\n");
    return 0;
  }

  window =
      SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       10 * SQUARE_WIDTH, 20 * SQUARE_WIDTH, 0);
  if (!window) {
    fprintf(stderr, "Error creating SDL Window.\n");
    return 0;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    fprintf(stderr, "Error creating SDL Renderer.\n");
    return 0;
  }

  return 1;
}

void process_input() {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
    case SDL_QUIT:
      game_running = 0;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        game_running = 0;
      } else if (event.key.keysym.sym == SDLK_LEFT) {
        position[0] -= 1;
        if (position[0] < 0) {
          position[0] = 0;
        }
      } else if (event.key.keysym.sym == SDLK_RIGHT) {
        position[0] += 1;
        if (position[0] + tetrominos[bag[current_index]].cols > 10) {
          position[0] = 10 - tetrominos[bag[current_index]].cols;
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
          if (position[1] + tetrominos[bag[current_index]].rows > 20) {
            position[1] = 20 - tetrominos[bag[current_index]].rows;
            break;
          }
        }
      }
      break;
  }
}

void setup() {
  shuffle_bag(bag, 7);
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 10; j++) {
      board_state[i][j].filled = 0;
    }
  }
}

int check_collisions(void) {
  const tetromino shape = tetrominos[bag[current_index]];

  for (int i = 0; i < shape.cols; i++) {
    for (int j = 0; j < shape.rows; j++) {
      if (shape.shape[j][i] == 1) {
        if (board_state[(int)position[1] + j + 1][(int)position[0] + i]
                .filled != 0) {
          return 1;  // Collision with another tetromino
        }
        if ((int)position[1] + j + 1 >= 20) {
          return 1;  // Reached the bottom of the board
        }
      }
    }
  }
  return 0;  // No collisions
}

int check_completed_lines(void) {
  int lines_cleared = 0;

  for (int i = 0; i < 20; i++) {
    int line_complete = 1;
    for (int j = 0; j < 10; j++) {
      if (board_state[i][j].filled == 0) {
        line_complete = 0;
        break;
      }
    }

    if (line_complete) {
      for (int j = 0; j < 10; j++) {
        board_state[i][j].filled = 0;
      }

      for (int k = i; k > 0; k--) {
        for (int j = 0; j < 10; j++) {
          board_state[k][j].filled = board_state[k - 1][j].filled;
          board_state[k][j].color = board_state[k - 1][j].color;
        }
      }

      lines_cleared++;
    }
  }

  return lines_cleared;
}

int game_over(void) {
  int game_over = 0;

  // Check each cell in the specified line
  for (int i = 0; i < 10; i++) {
    if (board_state[0][i].filled == 1) {
      game_over = 1;
      break;  // If any cell is filled, break the loop
    }
  }

  return game_over;
}

void update_board(void) {
  const tetromino shape = tetrominos[bag[current_index]];

  for (int i = 0; i < shape.cols; i++) {
    for (int j = 0; j < shape.rows; j++) {
      if (shape.shape[j][i] == 1) {
        board_state[(int)position[1] + j][(int)position[0] + i].filled = 1;
        board_state[(int)position[1] + j][(int)position[0] + i].color =
            shape.color;
      }
    }
  }

  int lines_cleared = check_completed_lines();
  if (lines_cleared > 0) {
    // add score stuff here
  }
}

void update() {
  // delta time, converted to seconds
  float delta_time = (SDL_GetTicks() - last_frame_time) / (float)1000;

  if (delta_time >= 1) {
    last_frame_time = SDL_GetTicks();

    const int blocks_per_sec = 1;

    int collision = check_collisions();

    if (collision) {
      update_board();
      position[1] = 0;
      current_index++;
      if (current_index >= 7) {
        shuffle_bag(bag, 7);
        current_index = 0;
      }
      position[1] = 0;
    } else {
      position[1] += blocks_per_sec * delta_time;
    }
  }
}

void render() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  const tetromino shape = tetrominos[bag[current_index]];

  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 20; j++) {
      SDL_Rect shape_rect = {(int)(i * SQUARE_WIDTH), (int)(j * SQUARE_WIDTH),
                             SQUARE_WIDTH, SQUARE_WIDTH};

      // adding grid lines
      SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
      SDL_RenderDrawRect(renderer, &shape_rect);

      if (board_state[j][i].filled == 1) {
        SDL_SetRenderDrawColor(
            renderer, board_state[j][i].color.r, board_state[j][i].color.g,
            board_state[j][i].color.b, board_state[j][i].color.a);
        SDL_RenderFillRect(renderer, &shape_rect);
      }
    }
  }

  SDL_SetRenderDrawColor(renderer, shape.color.r, shape.color.g, shape.color.b,
                         shape.color.a);
  for (int i = 0; i < shape.cols; i++) {
    for (int j = 0; j < shape.rows; j++) {
      if (shape.shape[j][i] == 1) {
        SDL_Rect shape_rect = {
            (int)(position[0] * SQUARE_WIDTH + i * SQUARE_WIDTH),
            (int)(position[1] * SQUARE_WIDTH + j * SQUARE_WIDTH), SQUARE_WIDTH,
            SQUARE_WIDTH};
        SDL_RenderFillRect(renderer, &shape_rect);
      }
    }
  }

  SDL_RenderPresent(renderer);
}

void destroy_window() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int main(void) {
  game_running = initialize_window();

  setup();

  while (game_running) {
    process_input();
    if (!game_over()) {  // Check for game over condition
      render();
      update();
    } else {
      // Game over logic here (e.g., display game over message)
      printf("Game Over!\n");
      game_running = 0;
    }
  }

  destroy_window();

  return 0;
}
