#pragma once
#include "model.h"

/***/
void process_input(int* game_running, float* position,
                   BoardCell (*board_state)[10], tetromino* current_piece,
                   int* dropped, int* rotation_state, size_t* score);
