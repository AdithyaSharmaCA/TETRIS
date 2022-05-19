#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <time.h> // to set speed of the game and to append levels
#include <math.h>
#include <stdlib.h> //random + more

#include <stdio.h> //to debug and to get err messages
#include <stdbool.h> //convenience


//definitions
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define ROWS 20
#define COLS 10
#define ROW_SIZE 30
#define COL_SIZE 30
#define ROW_NEXT 15
#define COL_NEXT 15


//these are official tetromino(piece) terms
//the last row is the color component
int shape_i[5][4] = {
    {1,1,1,1},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,255,255,255}
};

int shape_j[5][4] = {
    {1,0,0,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,255,255}
};

int shape_l[5][4] = {
    {0,0,1,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
    {255,127,0,255}
};

int shape_o[5][4] = {
    {0,1,1,0},
    {0,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
    {255,255,0,255}
};

int shape_s[5][4] = {
    {0,1,1,0},
    {1,1,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,255,0,255}
};

int shape_t[5][4] = {
    {0,1,0,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
    {128,0,128,255}
};

int shape_z[5][4] = {
    {1,1,0,0},
    {0,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
    {255,0,0,255}
};


//an array to randomly pick a shape
//piece_shape is an array of pointers, so it stores addresses
int (*piece_shape[7])[5][4] = {
    &shape_i,&shape_j,&shape_l,&shape_o,&shape_s,&shape_t,&shape_z
    };

//define a piece everytime with a random shape and color, so a struct
//id will be the shape address from piece_shape
//using said id, shape will be copied from piece_shape
typedef struct Piece{
    int id;
    int shape[5][4];
    int rotation;
    int x;
    int y;
}Piece;

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

//i.e, shape with colour, x, y etc
void get_piece_props(SDL_Renderer *renderer, Piece *P1) {
    srand(time(NULL)); //new seed
    P1->id = rand() %7; //rand shape id
    for (int i=0; i<5; i++){
        for (int j=0; j<4; j++){
            //P1's shape array will now have a copied version of the shape
            (P1->shape)[i][j] = (*piece_shape[P1->id])[i][j];
        }
    }

    //x and y of initial piece will start from the 4th grid and 1st col
    P1->x = (SCREEN_WIDTH / 2 - COLS*20) + (3*COL_SIZE);
    P1->y = COLS*5;
} 

//add collision checks here for when the piece rotates
void rotate_piece(int *piece[4][4]){

    int temp[4][4];

    //transpose, idk if this works
    for (int i =0; i<4; i++){
        for (int j =0; j<4; j++){
            temp[i][j] = *piece[i][j];
        }
    }

    for (int i =0; i<4; i++){
        for (int j =0; j<4; j++){
            *piece[i][j] = temp[i][j];
        }
    }

}

//movement restriction so that it won't move outside the grid
int collision_check(); 

//score + misc updates
void draw_ui_elements(); 

//draws a grid
void draw_grid(); 

//clears rows when a row is full from bottom to top
void clear_rows(); 

//draws the next piece on the sidebar
void draw_next_piece(SDL_Renderer *renderer){
    //get_next_piece();
    int next_pieces[6] = {0,1,2,3,4,5};
    for (int k = 0; k<6; k++){
        for (int i = 0; i<4; i++){
            for (int j = 0; j<4; j++){
                if ((*piece_shape[next_pieces[k]])[i][j]){
                    SDL_Rect rectNextPiece = {
                        //+8 is the x padding from the bg
                        //+4 is the x padding from the grid
                        .x = (SCREEN_WIDTH / 2 - COLS*20) + COL_SIZE*10 + 4 + (j*COL_NEXT) + 8,
                        .y = COLS*5 + ROW_SIZE*2 +i*ROW_NEXT + k*ROW_NEXT*4,
                        .w = COL_NEXT,
                        .h = ROW_NEXT
                    };

                    //getting the color from the shape array's 5th(last) row
                    int next_piece_R = (*piece_shape[next_pieces[k]])[4][0];
                    int next_piece_G = (*piece_shape[next_pieces[k]])[4][1];
                    int next_piece_B = (*piece_shape[next_pieces[k]])[4][2];
                    int next_piece_A = (*piece_shape[next_pieces[k]])[4][3];
                    SDL_SetRenderDrawColor(renderer, next_piece_R, next_piece_G, next_piece_B, next_piece_A);
                    SDL_RenderFillRect(renderer, &rectNextPiece);
                }
            }
        }
    }
    SDL_RenderPresent(renderer);
}

//draws the porjection of where the piece may land
void draw_piece_projection(); 

//wasd, c, space, esc keys
int get_key_input(); 

//check time to increase the drop speed of the pieces
int difficulty(); 

void main_menu(){

}

void gameplay(SDL_Renderer *renderer, int (*board)[COLS]) {

    Piece  P1;
    //we get a random shape for our piece along with x,y on the board
    get_piece_props(renderer, &P1);

    //background color
    //changes the color of the pallete
    SDL_SetRenderDrawColor(renderer, 0,120,120,255);
    //flushes the screen with whatever colour's selected
    SDL_RenderClear(renderer);

    //creates the grid
    SDL_SetRenderDrawColor(renderer, 10,10,10,255);
    SDL_Rect gridRect = {
        .x = SCREEN_WIDTH / 2 - COLS*20,
        .y = COLS*5,
        .w = COLS*COL_SIZE,
        .h = ROWS*ROW_SIZE
    };
    //then draws the grid
    SDL_RenderFillRect(renderer, &gridRect);

    //
    //we will display the pieces here so that grid lines will overlap the pieces
    //

    //should go inside gameplay
    //draw the piece
    //you need to draw this in a while loop and present it at the end of the loop
        //you keep adjusting the x and y based on key press and y-- offset
    for (int i = 0; i<4; i++){
        for (int j = 0; j<4; j++){
            if ((P1.shape)[i][j]){ //replace this with the piece shape from get_piece_props
                SDL_Rect rectPiece = {
                    .x = P1.x + (j*COL_SIZE),
                    .y = P1.y + i*ROW_SIZE,
                    .w = COL_SIZE,
                    .h = ROW_SIZE
                };

                //fetching the color of the pieces
                int cur_piece_R = P1.shape[4][0];
                int cur_piece_G = P1.shape[4][1];
                int cur_piece_B = P1.shape[4][2];
                int cur_piece_A = P1.shape[4][3]; //always a const, not needed
                SDL_SetRenderDrawColor(renderer, cur_piece_R, cur_piece_G, cur_piece_B, cur_piece_A);
                SDL_RenderFillRect(renderer, &rectPiece);
            }
        }
    }


    //vertical grid lines
    for (int i=0; i<((COLS+1)*COL_SIZE); i+=COL_SIZE) {
        //creating the grid lines
        SDL_SetRenderDrawColor(renderer, 45,45,45,255);
        SDL_Rect gridLines = {
            .x = (SCREEN_WIDTH / 2 - COLS*20) + i,
            .y = COLS*5,
            .w = 2,
            .h = ROWS*ROW_SIZE
        };
        //drawing them
        SDL_RenderFillRect(renderer, &gridLines);
    }

    //horizontal grid lines
    for (int i=0; i<((ROWS+1)*ROW_SIZE); i+=ROW_SIZE) {
        //creating the grid lines
        SDL_SetRenderDrawColor(renderer, 45,45,45,255);
        SDL_Rect gridLines = {
            .x = SCREEN_WIDTH / 2 - COLS*20,
            .y = (COLS*5) + i,
            .w = COLS*COL_SIZE,
            .h = 2
        };
        //drawing them
        SDL_RenderFillRect(renderer, &gridLines);
    }

    //creates the bg for the next pieces
    SDL_SetRenderDrawColor(renderer, 10,10,10,255);
    SDL_Rect gridNextRect = {
        .x = (SCREEN_WIDTH / 2 - COLS*20) + COL_SIZE*10 + 4,
        .y = COLS*5 + ROW_SIZE*1,
        .w = 5*COL_NEXT,
        .h = 25*ROW_NEXT
    };
    SDL_RenderFillRect(renderer, &gridNextRect);

    //to draw the next 3 pieces
    draw_next_piece(renderer);

    //outlines for the next pieces
    //vertical outlines
    for (int i=0; i<5*COL_NEXT; i+=COL_NEXT) {
        //creating the grid lines
        SDL_SetRenderDrawColor(renderer, 10,10,10,255);
        SDL_Rect gridLines = {
            //+8 is the x padding from the bg
            //+4 is the x padding from the grid
            .x = (SCREEN_WIDTH / 2 - COLS*20) + COL_SIZE*10 + 4 + i + 8, 
            .y = COLS*5 + ROW_SIZE*1,
            .w = 1,
            .h = 25*ROW_NEXT
        };
        //drawing them
        SDL_RenderFillRect(renderer, &gridLines);
    }

    //horizontal outlines
    for (int i=0; i<26*ROW_NEXT; i+=ROW_NEXT) {
        //creating the grid lines
        SDL_SetRenderDrawColor(renderer, 10,10,10,255);
        SDL_Rect gridLines = {
            .x = SCREEN_WIDTH / 2 - COLS*20 + COL_SIZE*10 + 4,
            .y = (COLS*5) + i + ROW_SIZE*1,
            .w = 5*COL_NEXT,
            .h = 1
        };
        //drawing them
        SDL_RenderFillRect(renderer, &gridLines);
    }

    //updates the screen from the backbuffer
    SDL_RenderPresent(renderer);

    //exit check
    bool running = true;
    SDL_Event event_running;
    while (running) {
        while(SDL_PollEvent(&event_running)) {
            if(event_running.type == SDL_QUIT){
                    running = false;
                    break;
            }
            else{
                //SDL_Delay(1000);
                //P1.y += ROW_SIZE;
                //goto label;
                //creating a goto is a terrible idea
                //it will hang the window
            }
        }
    }

}

void gameover();


int main(int argv, char** args) {

    //make the pieces offset --y so that it keeps falling,
        // get speed from difficulty()
    

    //initialize everything and then a check if it initialized properly
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("Error: SDL failed to initialize: '%s'\n", SDL_GetError());
        return 1;
    }

    //initialize sdl mixer
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048)==-1) {
        printf("Mix_OpenAudio: %s\n", Mix_GetError());
    }

    //load audio files
    Mix_Music *bgsound = Mix_LoadMUS("Tetris99.mp3");
    if (bgsound == NULL) {
        printf("Mix_LoadMUS: %s\n", Mix_GetError());
    }
    //Mix_Chunk *clear = Mix_LoadWAV("clear.wav"); //sound effects

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
    
    //adjust volume
    Mix_VolumeMusic(MIX_MAX_VOLUME*0.3);

    //play music
    if(Mix_PlayMusic(bgsound, -1)==-1) {
        printf("Mix_PlayMusic: %s\n", Mix_GetError());
    }

    //main menu screen
    main_menu();
    
    //create the board with 0 values
    int board[ROWS][COLS] = {0};

    //gameplay begins
    //we send the board's pointer to the function
    gameplay(renderer, board);

    //exit
    Mix_FreeMusic(bgsound);
    Mix_CloseAudio();
    //Mix_FreeChunk(clear);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(screen);
    SDL_Quit();

    return 0;
}

/*
Add the color components of the pieces to the shape arrays instead since they'll have fixed colors anyway
    this will be a pain since you'll have to add a 5th row which will mess up all for loops and such.
Random pick the id of the shape from the pieces_shape array
Then use said id to copy the shape into Piece.shape instead of passing array address
Then perform transpose etc to that copy instead


----------------------------------I------------------------------

Could create a 20x10 grid for the board for collision checks. 
    This will take care of collisions, line fill checks, drops etc.

This board/grid will need to have it's values set to 1 where the piece is there.
the piece will need to toggle 1's for where it is initially, at 4th col, 1st row
then a check will happen as to which of the board's cells are 1
these cells will then need to be filled with the respective color
which i suppose will need a 3rd dimension k


As for moving the piece,
idk yet


As for checking if lines are made

//eliminating lines
shift_board_down(int (*board)[COLS]){

    //the row below will take the values of the row above
    for (int i =ROWS-1; i>0; i--){
        for (int j = 0; j<COLS-1; j++){
            board[i][j] = board[i-1][j];
        }
    }

    //the first row will be emptied
    for (int j = 0; j<COLS-1; j++){
        board[0][j]=0;
    }

    //color doesn't matter since rendering won't begin if it's 0
}

//checking if lines are full
line_full_check(board);
line_full_check(int (*board)[COLS], int* score){
    for (int j=ROWS-1; j>0; j--){

        int row_elements[]={0};
        int count = 0;

        for (int i= 0; i<COLS-1;i++){
            row_elements[i] = (*board)[j][i];  //k the color var will be ignored
        }

        if(row_elements[i]){
            count++;
        }

        if (count == COLS){
            shift_board_down(&board);
            //append score after this loop to check if other lines were also eliminated
            //append a line counter that will take values up to 4 to record the bonus(?)
            //i think 4 lines = 1200 points
            //*score +=400
        }
    }
}

---------------------------------II------------------------------
or we could go the collision route.
drop:
we stop the pieces if the x value of the it is greater than the boundary location,
in which case we return out of the function, and bring the next piece in

collision:
we check each grid cell, that is the row_size and col_size, and check if any of the pieces is
on top of it. we'll have to record every single piece's x and y values into an array.
we remove these values when the piece is destroyed.
if it is on top, then it won't be able to move
if the bottom part of the piece collides with any piece, then we return out of the drop function,
record these values into the array
*/