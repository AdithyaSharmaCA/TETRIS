#ifndef Tetris_H
#define Tetris_H

#include "init.h"
#include "input.h"
#include "piece.h"
#include "draw.h"
#include "board.h"

bool main_menu();
void gameover(KeyInputs *Input, long long int *score);
bool gameplay(int (*board)[COLS][5]);

#endif