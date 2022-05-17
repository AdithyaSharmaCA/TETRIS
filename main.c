#include <SDL2/SDL.h>
#include <time.h> // to set speed of the game and to append levels
#include <math.h>
#include <stdlib.h> //random + more

#include <stdio.h> //to debug and to get err messages
#include <stdbool.h> //convinience


//definitions
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
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
int (*piece_shape[7])[4][4] = {
    &shape_i,&shape_j,&shape_l,&shape_o,&shape_s,&shape_t,&shape_z
    };

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

void main_menu(){

}

void gameplay();

void gameover();


int main(int argv, char** args) {

    //make the pieces offset --y so that it keeps falling,
        // get speed from difficulty()
    

    //initialize everything and then a check if it initialized properly
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("Error: SDL failed to initialize: '%s'\n", SDL_GetError());
        return 1;
    }

    //create the window
    SDL_Window *screen = SDL_CreateWindow(
        "TETRIS", 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        SCREEN_WIDTH, SCREEN_HEIGHT, 
        SDL_WINDOW_SHOWN
    );

    //checks if the window was created
    if (screen == NULL) {
        printf("Failed to create the window: %s\n", SDL_GetError());
        return 1;
    }

    //create the renderer
        //index -1 is the default gpu index
        //we use hardware acceleration and vsync
    SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1, 
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    


    //wrap everything that's below into the gameplay function


    //background color
    //changes the color of the pallete
    SDL_SetRenderDrawColor(renderer, 0,120,120,255);
    //flushes the screen with whatever colour's selected
    SDL_RenderClear(renderer);

    //creating the grid
    SDL_SetRenderDrawColor(renderer, 45,45,45,255);
    SDL_Rect gridRect = {
        .x = SCREEN_WIDTH / 2 - COLS*20,
        .y = COLS*5,
        .w = COLS*30,
        .h = ROWS*30
    };

    //draws the grid
    SDL_RenderFillRect(renderer, &gridRect);

    //updates the screen from the backbuffer
    SDL_RenderPresent(renderer);


    //main menu screen
    main_menu();


    //exit check
    bool running = true;
    SDL_Event event_running;
    while (running) {
        while(SDL_PollEvent(&event_running)) {
            switch(event_running.type) {
                case SDL_QUIT:
                    running = false;
                    break;
            }
        }
    }

    //exit
    SDL_DestroyWindow(screen);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
}