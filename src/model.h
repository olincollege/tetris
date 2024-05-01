#pragma once
#include <SDL2/SDL.h>

enum {
  SQUARE_WIDTH = 40,
  NUM_COLS = 10,
  NUM_ROWS = 20,
  NUM_TETROMINOS = 7,
  PADDING = 1,
  SCORE_AND_LEVEL_WIDTH = 4,
  WINDOW_NUM_COLS = 3 * PADDING + NUM_COLS + SCORE_AND_LEVEL_WIDTH,
  WINDOW_NUM_ROWS = 2 * PADDING + NUM_ROWS,
  LEVEL_POS_X = 12,
  LEVEL_POS_Y = 1,
  SCORE_POS_X = 12,
  SCORE_POS_Y = 2,
};

typedef struct {
  int horizontal;
  int vertical;
} direction;

typedef struct {
  int filled;
  SDL_Color color;
} BoardCell;

typedef struct tetromino {
  int rows;
  int cols;
  int shape[4][4];
  SDL_Color color;
  char letter;
} tetromino;

/**
 * Shuffle the bag of indices into the array of tetromino templates.
 *
 * The tetrominos are picked in the order of their indices in the bag. Once all
 * 7 pieces are picked, the bag is reshuffled.
 *
 * @param array The array of indices.
 * @param n The length of the bag array.
 */
void shuffle_bag(int* array, size_t n);

/**
 * Create an SDL window and associate an SDL renderer with it.
 *
 * @param renderer - a NULL pointer to be assigned a pointer to an SDL rendering
 * context.
 * @return 1 if setting up window and renderer are successful, 0 otherwise
 */
int initialize_window(SDL_Renderer** renderer);

/**
 * Shuffle start order of tetrominos, set board state, intialize first piece.
 *
 * The initial board state is filled with all 0s, indicating that there is no
 * tetromino piece occupying that square in the grid.
 *
 * @param board_state An array of pointers to arrays holding the contents of
 * each board row.
 * @param current_piece The currently falling tetromino struct.
 * @param position An array holding the x, y position of the piece.
 */
void setup(BoardCell (*board_state)[NUM_COLS], tetromino* current_piece,
           int* position);

/**
 * Copy the contents of a 4x4 template matrix into a 4x4 destination matrix.
 *
 * This function only copies the first `rows` rows and `cols` cols of the
 * template matrix.
 *
 * @param dest The destination matrix.
 * @param template The template matrix.
 * @param rows The number of rows to copy.
 * @param cols The number of columns to copy.
 */
void copy_shape_matrix(int dest[4][4], int template[4][4], int rows, int cols);

/**
 * Copy all fields of a template tetromino into the fields of the current piece.
 *
 * @param current_piece The currently falling tetromino struct.
 * @param position An array holding the x, y position of the piece.
 */
void set_current_piece(tetromino* current_piece, int* position);

/**
 * Set the position of the rotated piece based on its shape and oriantation.
 *
 * @param temp_position An array holding the x, y position of the temp piece.
 * @param current_piece The currently falling tetromino struct.
 * @param clockwise 1 if rotation is clockwise, 0 otherwise
 * @param rotation_state A number 0 to 3 indicating the orientation of the
 * piece.
 */
void set_temp_position(int* temp_position, tetromino* current_piece,
                       int clockwise, const int* rotation_state);

/**
 * Update the rotation state of the piece based on the rotation direction.
 *
 * @param clockwise 1 if rotation is clockwise, 0 otherwise
 * @param rotation_state A number 0 to 3 indicating the orientation of the
 * piece.
 */
void set_rotation_state(int clockwise, int* rotation_state);

/**
 * Rotate the shape matrix of the current tetromino piece.
 *
 * A temporary tetromino is created, and the rotation is tested with the
 * temporary piece and position. If there is space for the piece, the
 * current tetromino will be replaced with the temporary tetromino that has
 * a rotated shape matrix. The position of the tetromino is adjusted based
 * on the type of piece and which rotation state it is in.
 *
 * @param position An array holding the x, y position of the piece.
 * @param board_state An array of pointers to arrays holding the contents of
 * each board row.
 * @param current_piece The currently falling tetromino struct.
 * @param clockwise 1 if rotation is clockwise, 0 otherwise
 * @param rotation_state A number 0 to 3 indicating the orientation of the
 * piece.
 */
void rotate_shape(int* position, BoardCell (*board_state)[NUM_COLS],
                  tetromino* current_piece, int clockwise, int* rotation_state);

/**
 * Upon rotation, test if there is an available spot for the piece.
 *
 * This function implements a very basic version of wall kicking. If there is no
 * available space for the piece to rotate into the spot it should go, it will
 * test if there is space one spot to the right, one spot to the left, and one
 * spot up. It will update the position into the first available spot and
 * return 1. If there are no available spots, it will return 0.
 *
 * @param temp_position The position of where the rotated piece would end up.
 * @param board_state An array of pointers to arrays holding the contents of
 * each board row.
 * @param temp_tetromino The tetromino to test the rotation on.
 * @return 1 if the shape can be rotated, 0 otherwise.
 */
int available_position(int* temp_position, BoardCell (*board_state)[NUM_COLS],
                       tetromino* temp_tetromino);

/**
 * Check if the current falling piece will hit something in a given direction.
 *
 * @param position An array holding the x, y position of the piece.
 * @param board_state An array of pointers to arrays holding the contents of
 * each board row.
 * @param current_piece The currently falling tetromino struct.
 * @param dir The direction to check.
 * @return 1 if collision, 0 otherwise.
 */
int check_collisions(const int* position, BoardCell (*board_state)[NUM_COLS],
                     tetromino* current_piece, direction dir);

/**
 * Check for filled rows.
 *
 * Iterate through the board rows and check if each row is filled. If a row is
 * filled, shift the contents of all the rows above it down, clearing the row.
 *
 * @param board_state An array of pointers to arrays holding the contents of
 * each board row.
 * @param score The current game score.
 * @return The number of lines cleard.
 */
size_t check_completed_lines(BoardCell (*board_state)[NUM_COLS]);

/**
 * Check for game over condition.
 *
 * If the top row of the board is filled, the game is over.
 *
 * @param board_state An array of pointers to arrays holding the contents of
 * each board row.
 * @return 1 if game is over, 0 otherwise
 */
int game_over(BoardCell (*board_state)[NUM_COLS]);

/**
 * Add a fallen piece to the board state and check for complete lines.
 *
 * @param position An array holding the x, y position of the piece.
 * @param board_state An array of pointers to arrays holding the contents of
 * each board row.
 * @param current_piece The currently falling tetromino struct.
 * @param score The current game score.
 * @param level The current level.
 */
void update_board(const int* position, BoardCell (*board_state)[NUM_COLS],
                  tetromino* current_piece, size_t* score, size_t* level);

/**
 * Determine the time interval for the piece to fall based on the level.
 *
 * @param level The current level.
 * @return The time interval to wait before dropping the current piece one row.
 */
float get_time_int(const size_t* level);

/**
 * Each time interval, drop the current piece down.
 *
 * If the current piece can't be dropped (it has hit the bottom or another
 * piece), spawn a new shape at the top of the screen and reset rotation state
 * to 0.
 *
 * @param position An array holding the x, y position of the piece.
 * @param board_state An array of pointers to arrays holding the contents of
 * each board row.
 * @param current_piece The currently falling tetromino struct.
 * @param dropped 1 if the piece was hard dropped, 0 otherwise.
 * @param rotation_state A number 0 to 3 indicating the orientation of the
 * piece.
 * @param score The current game score.
 * @param level The current level.
 */
void update(int* position, BoardCell (*board_state)[NUM_COLS],
            tetromino* current_piece, int* dropped, int* rotation_state,
            size_t* score, size_t* level);

/**
 * Destory renderer, window, and clean up SDL processes.
 *
 * @param renderer The 2D rendering context for the SDL window.
 */
void destroy_window(SDL_Renderer* renderer);
