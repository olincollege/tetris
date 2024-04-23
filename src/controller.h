#pragma once
#include "model.h"

/**
 * Take keyboard input and move falling tetromino accordingly.
 *
 * Listen for SDL events. If the event is Quit or Escape, stop the game. If the
 * event is a left or right arrow, translate the currently following tetromino
 * in that direction, if there is space. If the event is an up arrow or z key,
 * rotate the piece clockwise or counterclockwise, respectively, if there is
 * space. If the event is a down arrow, move the piece down one row, if space,
 * and if the event is a space key, drop the piece until it hits something.
 *
 * @param game_running 1 if the game is running, 0 otherwise.
 * @param position An array holding the x, y position of the piece.
 * @param board_state An array of pointers to arrays holding the contents of
 * each board row.
 * @param current_piece The currently falling tetromino struct.
 * @param dropped 1 if the piece was hard dropped, 0 otherwise.
 * @param rotation_state A number 0 to 3 indicating the orientation of the
 * piece.
 * @param score The current score.
 */
void process_input(int* game_running, float* position,
                   BoardCell (*board_state)[10], tetromino* current_piece,
                   int* dropped, int* rotation_state, size_t* score);
