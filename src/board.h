#ifndef BOARD_H
#define BOARD_H

#include "input.h" //for gameover
#include "piece.h"

bool collisionCheck(const int (*board)[COLS][5], const Piece *P1);
void shiftBoardDown(int (*board)[COLS][5], const int row_index);
void lineFullCheck(int (*board)[COLS][5], long long int *score);
void addPieceToBoard(int (*board)[COLS][5], Piece* P1);
int dropCheck(int (*board)[COLS][5], Piece *P1, long long int *score, int next_pieces[6]);

#endif