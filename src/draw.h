#ifndef DRAW_H
#define DRAW_H

#include "init.h"

//unnecessary
void drawGridBackground();
void drawPiece(const Piece *P1);
void drawBoardPieces(const Piece *P1, const int (*board)[COLS][5]);
void drawBoardGridLines();
void drawNextPiece(const int next_pieces[6]);
void drawNextGrid(const int next_pieces[6]);
void drawUiElements(const long long int *score);
void drawGarbageGrid();
void drawGarbageCells(int garbage[COLS]);
void drawPieceProjection();
//

void drawEverything(Piece *P1, int next_pieces[6], int (*board)[COLS][5], long long int *score, int *garbage);

#endif