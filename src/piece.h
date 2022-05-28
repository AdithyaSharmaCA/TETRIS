#ifndef PIECE_H
#define PIECE_H

#include "defs.h"

void getPieceProps(Piece *P1, const int check);
int nextPieceShift(Piece *P1, int next_pieces[6]);
void rotatePiece(Piece *P1, const int direction);
int difficulty();

#endif