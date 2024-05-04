
#include "model.h"

#include <SDL2/SDL.h>
#include <math.h>

SDL_Window* window = NULL;

int current_index = 0;
int bag[] = {0, 1, 2, 3, 4, 5, 6};

const tetromino tetrominos[NUM_TETROMINOS] = {  // I
    {.rows = 1,
     .cols = 4,
     .shape = {{1, 1, 1, 1}},
     .color = {.r = 0, .g = 255, .b = 255, .a = 255},
     .letter = 'I'},
    // J
    {.rows = 2,
     .cols = 3,
     .shape = {{1, 0, 0}, {1, 1, 1}},
     .color = {.r = 0, .g = 0, .b = 255, .a = 255},
     .letter = 'J'},
    // L
    {.rows = 2,
     .cols = 3,
     .shape = {{0, 0, 1}, {1, 1, 1}},
     .color = {.r = 255, .g = 165, .b = 0, .a = 255},
     .letter = 'L'},
    // O
    {.rows = 2,
     .cols = 2,
     .shape = {{1, 1}, {1, 1}},
     .color = {.r = 255, .g = 255, .b = 0, .a = 255},
     .letter = 'O'},
    // S
    {.rows = 2,
     .cols = 3,
     .shape = {{0, 1, 1}, {1, 1, 0}},
     .color = {.r = 0, .g = 255, .b = 0, .a = 255},
     .letter = 'S'},
    // T
    {.rows = 2,
     .cols = 3,
     .shape = {{0, 1, 0}, {1, 1, 1}},
     .color = {.r = 128, .g = 0, .b = 255, .a = 255},
     .letter = 'T'},
    // Z
    {.rows = 2,
     .cols = 3,
     .shape = {{1, 1, 0}, {0, 1, 1}},
     .color = {.r = 255, .g = 0, .b = 0, .a = 255},
     .letter = 'Z'}};

float last_frame_time = 0;

void shuffle_bag(int* array, size_t n) {
  if (n > 1) {
    for (size_t i = 0; i < n - 1; i++) {
      size_t random_int =
          (size_t)rand();  // NOLINT(cert-msc30-c, cert-msc50-cpp)
      size_t swap_ind = i + random_int / (RAND_MAX / (n - i) + 1);
      int swap_val = array[swap_ind];
      array[swap_ind] = array[i];
      array[i] = swap_val;
    }
  }
}

int initialize_window(SDL_Renderer** renderer) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    (void)fprintf(stderr, "Error initializing SDL.\n");
    return 0;
  }

  window = SDL_CreateWindow(
      NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      WINDOW_NUM_COLS * SQUARE_WIDTH, WINDOW_NUM_ROWS * SQUARE_WIDTH, 0);
  if (!window) {
    (void)fprintf(stderr, "Error creating SDL Window.\n");
    return 0;
  }

  *renderer = SDL_CreateRenderer(window, -1, 0);
  if (!*renderer) {
    (void)fprintf(stderr, "Error creating SDL Renderer.\n");
    return 0;
  }

  return 1;
}

void setup(BoardCell (*board_state)[NUM_COLS], tetromino* current_piece,
           int* position) {
  shuffle_bag(bag, NUM_TETROMINOS);
  for (int i = 0; i < NUM_ROWS; i++) {
    for (int j = 0; j < NUM_COLS; j++) {
      board_state[i][j].filled = 0;
    }
  }
  set_current_piece(current_piece, position);
}

void copy_shape_matrix(int dest[4][4], int rows, int template[4][4], int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      dest[i][j] = template[i][j];
    }
  }
}

void set_current_piece(tetromino* current_piece, int* position) {
  // if I piece, spawn one space lower
  if (bag[current_index] == 0) {
    position[1] = 1;
  }
  tetromino template = tetrominos[bag[current_index]];
  current_piece->rows = template.rows;
  current_piece->cols = template.cols;
  current_piece->color = template.color;
  current_piece->letter = template.letter;
  copy_shape_matrix(current_piece->shape, template.rows, template.shape,
                    template.cols);
}

void set_temp_position(int* temp_position, tetromino* current_piece,
                       int clockwise, const int* rotation_state) {
  if (current_piece->rows < current_piece->cols) {
    temp_position[1] -= 1;
  } else {
    temp_position[1] += 1;
  }

  if (current_piece->letter == 'I') {
    if (*rotation_state % 2 == 0) {
      temp_position[0] += 2;
    } else {
      temp_position[0] -= 2;
    }
  } else if (current_piece->letter == 'Z') {
    if (*rotation_state % 2 == 0) {
      temp_position[0] += 1;
    } else {
      temp_position[0] -= 1;
    }
  } else if (current_piece->letter == 'L' || current_piece->letter == 'J' ||
             current_piece->letter == 'T') {
    if (*rotation_state == 1) {
      temp_position[0] -= 1;
    } else if ((*rotation_state == 0 && clockwise) ||
               (*rotation_state == 2 && !clockwise)) {
      temp_position[0] += 1;
    }
  }
}

void set_rotation_state(int clockwise, int* rotation_state) {
  if (!clockwise) {
    *rotation_state = *rotation_state - 1 == -1 ? 3 : *rotation_state - 1;
  } else {
    *rotation_state = *rotation_state + 1 == 4 ? 0 : *rotation_state + 1;
  }
}

void rotate_shape(int* position, BoardCell (*board_state)[NUM_COLS],
                  tetromino* current_piece, int clockwise,
                  int* rotation_state) {
  if (current_piece->letter != 'O') {
    int temp_shape[4][4];
    int temp_position[] = {position[0], position[1]};

    for (int i = 0; i < current_piece->rows; i++) {
      for (int j = 0; j < current_piece->cols; j++) {
        // left (counterclockwise)
        if (clockwise == 0) {
          temp_shape[current_piece->cols - j - 1][i] =
              current_piece->shape[i][j];
        } else {
          temp_shape[j][current_piece->rows - i - 1] =
              current_piece->shape[i][j];
        }
      }
    }

    set_temp_position(temp_position, current_piece, clockwise, rotation_state);

    tetromino temp_tetromino = {.rows = current_piece->cols,
                                .cols = current_piece->rows,
                                .color = current_piece->color};
    copy_shape_matrix(temp_tetromino.shape, current_piece->cols, temp_shape,
                      current_piece->rows);

    if (available_position(temp_position, board_state, &temp_tetromino)) {
      current_piece->cols = temp_tetromino.cols;
      current_piece->rows = temp_tetromino.rows;
      copy_shape_matrix(current_piece->shape, temp_tetromino.rows,
                        temp_tetromino.shape, temp_tetromino.cols);

      position[0] = temp_position[0];
      position[1] = temp_position[1];

      set_rotation_state(clockwise, rotation_state);
    }
  }
}

int available_position(int* temp_position, BoardCell (*board_state)[NUM_COLS],
                       tetromino* temp_tetromino) {
  direction dir_in_place = {.horizontal = 0, .vertical = 0};
  direction dir_right = {.horizontal = 1, .vertical = 0};
  direction dir_left = {.horizontal = -1, .vertical = 0};
  direction dir_up = {.horizontal = 0, .vertical = -1};

  if (!check_collisions(temp_position, board_state, temp_tetromino,
                        dir_in_place)) {
    return 1;
  }
  if (!check_collisions(temp_position, board_state, temp_tetromino,
                        dir_right)) {
    temp_position[0] += 1;
    return 1;
  }
  if (!check_collisions(temp_position, board_state, temp_tetromino, dir_left)) {
    temp_position[0] -= 1;
    return 1;
  }
  if (!check_collisions(temp_position, board_state, temp_tetromino, dir_up)) {
    temp_position[1] -= 1;
    return 0;
  }
  return 0;
}

int check_collisions(const int* position, BoardCell (*board_state)[NUM_COLS],
                     tetromino* current_piece, direction dir) {
  for (int i = 0; i < current_piece->cols; i++) {
    for (int j = 0; j < current_piece->rows; j++) {
      if (current_piece->shape[j][i] == 1 &&
          board_state[position[1] + j + dir.vertical]
                     [position[0] + i + dir.horizontal]
                         .filled != 0) {
        return 1;  // Collision with another tetromino
      }
      if (current_piece->shape[j][i] == 1 &&
          position[1] + j + dir.vertical >= NUM_ROWS) {
        return 1;  // Reached the bottom of the board
      }
      if (current_piece->shape[j][i] == 1 && position[0] + dir.horizontal < 0) {
        return 1;  // off the left side
      }
      if (current_piece->shape[j][i] == 1 &&
          position[0] + current_piece->cols + dir.horizontal > NUM_COLS) {
        return 1;  // off the right side
      }
      if (current_piece->shape[j][i] == 1 && position[1] < 0) {
        return 1;  // off the top
      }
    }
  }
  return 0;  // No collisions
}

size_t check_completed_lines(BoardCell (*board_state)[NUM_COLS]) {
  size_t lines_cleared = 0;

  for (int i = 0; i < NUM_ROWS; i++) {
    int line_complete = 1;
    for (int j = 0; j < NUM_COLS; j++) {
      if (board_state[i][j].filled == 0) {
        line_complete = 0;
        break;
      }
    }

    if (line_complete) {
      for (int j = 0; j < NUM_COLS; j++) {
        board_state[i][j].filled = 0;
      }

      for (int k = i; k > 0; k--) {
        for (int j = 0; j < NUM_COLS; j++) {
          board_state[k][j].filled = board_state[k - 1][j].filled;
          board_state[k][j].color = board_state[k - 1][j].color;
        }
      }

      lines_cleared++;
    }
  }
  return lines_cleared;
}

int game_over(BoardCell (*board_state)[NUM_COLS]) {
  int game_over = 0;

  // Check each cell in the specified line
  for (int i = 0; i < NUM_COLS; i++) {
    if (board_state[0][i].filled == 1) {
      game_over = 1;
      break;  // If any cell is filled, break the loop
    }
  }

  return game_over;
}

void update_board(const int* position, BoardCell (*board_state)[NUM_COLS],
                  tetromino* current_piece, size_t* score, size_t* level,
                  size_t* total_lines_cleared) {
  for (int i = 0; i < current_piece->cols; i++) {
    for (int j = 0; j < current_piece->rows; j++) {
      if (current_piece->shape[j][i] == 1) {
        board_state[position[1] + j][position[0] + i].filled = 1;
        board_state[position[1] + j][position[0] + i].color =
            current_piece->color;
      }
    }
  }

  size_t lines_cleared = check_completed_lines(board_state);
  *total_lines_cleared += lines_cleared;
  *level = *total_lines_cleared / LINES_PER_LEVEL + 1;

  if (lines_cleared != 0) {
    *score +=
        lines_cleared == 4
            ? FOUR_LINE_PTS * (*level)
            : (DOUBLE_ONE_LINE * lines_cleared - ONE_LINE_OFFSET) * (*level);
  }
}

float get_time_int(const size_t* level) {
  const float max_base = .8f;
  const float sub_scale_factor = .007f;
  float base = (max_base - ((float)(*level - 1) * sub_scale_factor));
  float exponent = (float)(*level - 1);
  float res = powf(base, exponent);

  return res;
}

void update(int* position, BoardCell (*board_state)[NUM_COLS], int* dropped,
            tetromino* current_piece, int* rotation_state, size_t* score,
            size_t* level, size_t* total_lines_cleared) {
  // delta time, converted to seconds
  float delta_time =
      ((float)SDL_GetTicks() - last_frame_time) / (float)MS_IN_SEC;

  if (delta_time >= get_time_int(level) || *dropped == 1) {
    last_frame_time = (float)SDL_GetTicks();

    direction dir_down = {.horizontal = 0, .vertical = 1};
    int collision =
        check_collisions(position, board_state, current_piece, dir_down);

    if (collision) {
      update_board(position, board_state, current_piece, score, level,
                   total_lines_cleared);
      position[0] = 3;
      position[1] = 0;
      (current_index)++;
      if (current_index >= NUM_TETROMINOS) {
        shuffle_bag(bag, NUM_TETROMINOS);
        current_index = 0;
      }
      set_current_piece(current_piece, position);
      *rotation_state = 0;
    } else {
      position[1] += 1;
    }
    *dropped = 0;
  }
}

void destroy_window(SDL_Renderer* renderer) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();
}
