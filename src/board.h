#ifndef BOARD_H
#define BOARD_H

#include "input.h" //for gameover
#include "piece.h"

extern int linesCompleted;
extern int linesToAdd;
extern TCPsocket client;
extern SDL_mutex *mut;

bool collisionCheck(const int (*board)[COLS][5], const Piece *P1);
void shiftBoardDown(int (*board)[COLS][5], const int row_index);
void lineFullCheck(int (*board)[COLS][5], long long int *score, int *linesCleared);
void addJunkLines(int (*board)[COLS][5], int voidCell, Piece *P1);
void addPieceToBoard(int (*board)[COLS][5], Piece* P1);
int dropCheck(int (*board)[COLS][5], Piece *P1, long long int *score, int next_pieces[6]);

#endif