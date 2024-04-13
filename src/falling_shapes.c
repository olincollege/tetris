#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

int game_running = 0;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int last_frame_time = 0;

const int SQUARE_WIDTH = 40;
float position[] = {3, 0};
int bag[] = {0, 1, 2, 3, 4, 5, 6};

typedef struct tetromino {
  int rows;
  int cols;
  int (*shape)[][];
  SDL_Color color;
} tetromino;

// int* make_pointer_to_tetromino_shape(int height, int width) {
//   int* shape = (int*)malloc(height * sizeof(int[width]));
//   return shape;
// }

// void free_tetromino_shape(int* shape) { free(shape); }

int I[2][4] = {{0, 0, 0, 0}, {1, 1, 1, 1}};
int J[2][3] = {{1, 0, 0}, {1, 1, 1}};
int L[2][3] = {{0, 0, 1}, {1, 1, 1}};
int S[2][3] = {{0, 1, 1}, {1, 1, 0}};
int O[2][2] = {{1, 1}, {1, 1}};
int Z[2][3] = {{1, 1, 0}, {0, 1, 1}};
int T[2][3] = {{0, 1, 0}, {1, 1, 1}};
int* tetromino_shape_templates[] = {I, J, L, O, S, T, Z};

tetromino tetrominos[7] = {
    // I
    {.rows = 2,
     .cols = 4,
     .shape = I,
     .color = {.r = 0, .g = 255, .b = 255, .a = 255}},
    // J
    {.rows = 2,
     .cols = 3,
     .shape = J,
     .color = {.r = 0, .g = 0, .b = 255, .a = 255}},
    // L
    {.rows = 2,
     .cols = 3,
     .shape = L,
     .color = {.r = 255, .g = 165, .b = 0, .a = 255}},
    // O
    {.rows = 2,
     .cols = 2,
     .shape = O,
     .color = {.r = 0, .g = 255, .b = 255, .a = 0}},
    // S
    {.rows = 2,
     .cols = 3,
     .shape = S,
     .color = {.r = 0, .g = 255, .b = 0, .a = 255}},
    // T
    {.rows = 2,
     .cols = 3,
     .shape = T,
     .color = {.r = 128, .g = 0, .b = 255, .a = 255}},
    // Z
    {.rows = 2,
     .cols = 3,
     .shape = Z,
     .color = {.r = 255, .g = 0, .b = 0, .a = 255}}};

void allocate_tetromino_shape(tetromino tetromino, int tetromino_index) {
  int** shape = tetromino.shape;
  int rows = tetromino.rows;
  int cols = tetromino.cols;
  *shape = malloc(sizeof(int) * rows * cols);
  memcpy(*shape, tetromino_shape_templates[tetromino_index],
         sizeof(int) * rows * cols);
}

int board_state[20][10] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

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
      }
      break;
  }
}

void setup() {
  // for (int i = 0; i < 7; i++) {
  //   allocate_tetromino_shape(tetrominos[i], i);
  // }
  shuffle_bag(bag, 7);
}

void update() {
  // delta time, converted to seconds
  float delta_time = (SDL_GetTicks() - last_frame_time) / (float)1000;

  if (delta_time >= 1) {
    last_frame_time = SDL_GetTicks();

    const int blocks_per_sec = 1;
    if (position[1] > 19) {
      position[1] = 0;
    } else {
      position[1] += blocks_per_sec * delta_time;
    }
  }
}

int current_index = 0;

void render() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // Render the tetromino at the current index in the bag
  const tetromino shape = tetrominos[bag[current_index]];

  SDL_SetRenderDrawColor(renderer, shape.color.r, shape.color.g, shape.color.b,
                         shape.color.a);

  for (int i = 0; i < shape.rows; i++) {
    for (int j = 0; j < shape.cols; j++) {
      if ((*shape.shape)[j] != 0) {
        SDL_Rect shape_rect = {
            (int)(position[0] * SQUARE_WIDTH + i * SQUARE_WIDTH),
            (int)(position[1] * SQUARE_WIDTH + j * SQUARE_WIDTH), SQUARE_WIDTH,
            SQUARE_WIDTH};
        SDL_RenderFillRect(renderer, &shape_rect);
      }
    }
  }

  SDL_RenderPresent(renderer);

  // If the current tetromino has left the screen, move to the next shape
  if (position[1] > 19) {
    current_index++;
    if (current_index >= 7) {
      // If all tetrominos have been rendered, shuffle the bag
      shuffle_bag(bag, 7);  // Assuming there are 7 tetrominos
      current_index = 0;
    }
    position[1] = 0;
  }
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
    render();
    update();
  }

  destroy_window();

  return 0;
}
