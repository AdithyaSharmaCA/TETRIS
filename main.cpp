//#include <insert_gui_library>
#include <SDL2/SDL.h>
#include <time.h> // to set speed of the game and to append levels
#include <math.h>
#include <stdlib.h> //random + more


//definitions
#define ROWS 20
#define COLS 10


//these are official tetromino(piece) terms
int shape_i[4][4] = {
    {0,0,0,0},
    {0,0,0,0},
    {1,1,1,1},
    {0,0,0,0}
};

int shape_j[4][4] = {
    {0,0,0,0},
    {1,0,0,0},
    {1,1,1,0},
    {0,0,0,0}
};

int shape_l[4][4] = {
    {0,0,0,0},
    {0,0,1,0},
    {1,1,1,0},
    {0,0,0,0}
};

int shape_o[4][4] = {
    {0,0,0,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,0,0,0}
};

int shape_s[4][4] = {
    {0,0,0,0},
    {0,0,1,1},
    {0,1,1,0},
    {0,0,0,0}
};

int shape_t[4][4] = {
    {0,0,0,0},
    {0,1,0,0},
    {1,1,1,0},
    {0,0,0,0}
};

int shape_z[4][4] = {
    {0,0,0,0},
    {1,1,0,0},
    {0,1,1,0},
    {0,0,0,0}
};


//an array to randomly pick a shape
//piece_shape is an array of pointers, so it stores addresses
int (*piece_shape[7])[4][4]={&shape_i,&shape_j,&shape_l,&shape_o,&shape_s,&shape_t,&shape_z};

//an array to randomly pick a color
//add rgb values depending on gui library
int *piece_color[7];


//define a piece everytime with a random shape and color, so a struct
struct Piece{
    int color;
    int shape;
    int rotation;
};

//might be better to #define keyinputs instead
//struct's only useful if you want to create profiles
struct KeyInputs{
    int left;       //move piece left , (Left arrow key)
    int right;      //move piece right, (Right arrow key)
    int rotate_l;   //rotate anti-clockwise , (Up arrow key)
    int rotate_r;   //rotate clockwise , (Down arrow key)
    int soft_drop;  //(Shift or alt)
    int hard_drop;  //(spacebar)
    int hold;       //hold piece or swap piece, (C)
    int quit;       //exit to main menu(only for debugging) (Esc)
};

//initialize game window using library command
void initialize_window(); 

//i.e, shape, colour, etc
int get_piece_props(); 

//add collision checks here for when the piece rotates
void rotate_piece(); 

//movement restriction so that it won't move outside the grid
int collision_check(); 

//score + misc updates
void draw_ui_elements(); 

//draws a grid
void draw_grid(); 

//clears rows when a row is full from bottom to top
void clear_rows(); 

//draws the next piece on the sidebar
void draw_next_piece(); 

//draws the porjection of where the piece may land
void draw_piece_projection(); 

//wasd, c, space, esc keys
int get_key_input(); 

//check time to increase the drop speed of the pieces
int difficulty(); 

void main_menu();

void gameplay();

void gameover();

int main(int argv, char** args)
{
    //main_menu();

    //make the pieces offset --y so that it keeps falling,
        // get speed from difficulty()
    
    //exit while loop so that the window doesn't close
    return 0;
}