#pragma once
#include "model.h"

/***/
void process_input(int* game_running, float* position, int* current_index,
                   tetromino* tetrominos, int* bag,
                   BoardCell (*board_state)[10]);
