#ifndef defs_H
#define defs_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <time.h> // to set speed of the game and to append levels
#include <stdlib.h> //random + more
#include <stdio.h> //to debug and to get err messages
#include <stdbool.h> //convenience

/*this is what stdbool does kek
#define true 1
#define false 0
*/

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define ROWS 20
#define COLS 10
#define ROW_SIZE 30
#define COL_SIZE 30
#define ROW_NEXT 15
#define COL_NEXT 15

extern const int board_starting_x;
extern const int board_starting_y;

extern const int (*piece_shape[7])[5][4];

//id will be the shape address from piece_shape
//using said id, shape will be copied from piece_shape
typedef struct Piece{
    int id;
    int shape[5][4];
    int x;
    int y;
}Piece;


#endif