#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
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

int board_starting_x = (SCREEN_WIDTH / 2 - COLS*20);
int board_starting_y = COLS*5;


//these are official tetromino(piece) terms
//the last row is the color component
int shape_i[5][4] = {
    {0,0,0,0},
    {0,0,0,0},
    {1,1,1,1},
    {0,0,0,0},
    {0,255,255,255}
};

int shape_j[5][4] = {
    {0,0,0,0},
    {1,0,0,0},
    {1,1,1,0},
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
    {0,0,0,0},
    {0,1,1,0},
    {0,1,1,0},
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
//check var here checks if it's the first piece or not
    //if it is the next piece after being placed, then that will be the id
void get_piece_props(Piece *P1, int check) {
    if (check <0){
        P1->id = rand() %7; //rand shape id
    }
    else{
        P1->id = check; // id from next piece array
    }
    for (int i=0; i<5; i++){
        for (int j=0; j<4; j++){
            //P1's shape array will now have a copied version of the shape
            //so that it doesn't mess with the original after rotation
            (P1->shape)[i][j] = (*piece_shape[P1->id])[i][j];
        }
    }

    //x and y of initial piece will start from the 4th grid and 1st col
    P1->x = board_starting_x + (3*COL_SIZE);
    P1->y = board_starting_y;
} 

//add collision checks here for when the piece rotates
void rotate_piece(Piece *P1){

    int temp[4][4];

    //we have to limit the pivot of the piece
    //so it'll be 3 for all pieces other than I and O
    int n = 3;
    if(P1->id == 0 || P1->id == 3){
        n =4;
    }

    for (int i =0; i<4; i++){
        for (int j =0; j<4; j++){
            temp[i][j] = P1->shape[i][j];
        }
    }

    //then we rotate it 90 degress within the 3x3 constraint
    for (int i =0; i<n; i++){
        for (int j =0; j<n; j++){
            P1->shape[i][j] = temp[n-j-1][i];
            //P1->shape[n-i-1][n-j-1] = temp[j][i]; //anti clockwise
        }
    }
}

//after a piece has been placed
int next_piece_shift(Piece *P1, int next_pieces[6]){
    //when we add the piece to the grid after collision or when we hold the piece
    //or when we hard drop the piece,
    //collision_check(&P1, next_pieces);

    //P1 is already a pointer, so i don't think &P1 is necessary
    get_piece_props(P1, next_pieces[0]);

    for (int i = 0; i < (6-1); i++){
        next_pieces[i] = next_pieces[i+1];
    }
    int next_id = rand() %7; //rand shape id
    next_pieces[5] = next_id;

    //we move all the elements by 1 index and then 
        //fill the last element with a random id
}

//checking if the piece soft dropped into the bottom row of the grid
int piece_board_collision_check(Piece *P1, int (*board)[COLS][5], int next_pieces[6]){

    //starting x and y coord of the board from which when we subtract the x,y
    //coords of the piece, we get the row and column that the piece is in of the board

    int i = ( (P1->y - board_starting_y) / ROW_SIZE );   //row where y is at
    int j = ( (P1->x - board_starting_x) / COL_SIZE ); //col where x is at

    // we check if the current x,y coords of the piece on the board is equal to the
    // board's last row. and we decide the last row of the board with a for loop
    //by checking if any piece from the bottom of the matrix is occupied
    //b/c in the 4x4 shape matrix, pieces can be rotated and the last row can be empty
    //then we add the color component to the board grid for it to be rendered

    int last_row = 16;
    bool exit_check = false;
    for(int k=3;k>=0;k--){
        for (int l=0;l<4;l++){
            if(P1->shape[k][l]){
                if(k>0){
                    last_row += (3-k);
                    exit_check = true;
                    break;
                }
            }
            if(exit_check) break;
        }
        if(exit_check) break;
    }

    if(board[i][j] == board[last_row][j]){
        for (int k =0; k<4; k++){
            for (int l =0; l<4; l++){
                if ( P1->shape[k][l] ){
                    //piece is now occupied at the grid spot
                    board[i+k][j+l][4] = 1;
                    //color component
                    board[i+k][j+l][0] = P1->shape[4][0];
                    board[i+k][j+l][1] = P1->shape[4][1];
                    board[i+k][j+l][2] = P1->shape[4][2];
                    board[i+k][j+l][3] = P1->shape[4][3];                    
                }
            }
        }
        //fetches the next piece
        next_piece_shift(P1, next_pieces);
        return 0;
    }

    //collision with other pieces
    for (int c = 3; c>=0; c--){
       for (int d = 0; d<4; d++){
            if(board[i+c+1][j+d][4] && P1->shape[c][d]){
                for (int a = 0; a<4; a++){
                        for (int b = 0; b<4; b++){
                            if(P1->shape[a][b]){
                                //piece is now occupied at the grid spot
                                board[i+a][j+b][4] = 1;
                                //color component
                                board[i+a][j+b][0] = P1->shape[4][0];
                                board[i+a][j+b][1] = P1->shape[4][1];
                                board[i+a][j+b][2] = P1->shape[4][2];
                                board[i+a][j+b][3] = P1->shape[4][3];
                            }
                        }
                    }
                //fetches the next piece
                next_piece_shift(P1, next_pieces);
                return 0; 
            }
        }
    }
}

//movement restriction so that it won't move outside the grid

bool sideway_collision_check(Piece *P1 , int direction, int (*board)[COLS][5]){
    //checks if it collides with the board edges

    int colLoc = ( (P1->x - board_starting_x) / COL_SIZE );
    int rowLoc = ( (P1->y - board_starting_y) / ROW_SIZE );
    int colStart = 0, colEnd = 0;
    
    bool startFound = false;
    bool endFound = false;

    //starting loc that the piece is in, inside the shape
    for (int i = 0; i<4; i++){
        for (int j = 0; j<4; j++){
            if(P1->shape[j][i]){
                colStart = i;
                startFound = true;
                break;
            }
            if(startFound) break;
        }
        if(startFound) break;
    }
    //pos on the board
    colStart = colStart + colLoc;

    //end loc that the piece is in
    for (int i = 4-1; i>=0; i--){
        for (int j = 0; j<4; j++){
            if(P1->shape[j][i]){
                colEnd = i;
                endFound = true;
                break;
            }
            if(endFound) break;
        }
        if(endFound) break;
    }

    //colEnd = colLoc + 4 - colEnd + 1;
    colEnd = colLoc +colEnd + 1;
    
    //move left possible?
    if (direction == 1){
        if( !(colStart>0) ){
            return false;
        }
    }

    //move right possible?
    else if(direction == 2){
        if( !(colEnd<10) ){
            return false;
        }
    }

    //get back board x and y locs
    //colStart -=colLoc;
    //colEnd -=colLoc;

    colLoc = ( (P1->x - board_starting_x) / COL_SIZE );

    printf("%d %d %d\n", colLoc, colStart, colEnd);
    //checks if it collides with any other piece sideways
    if (direction == 1){
        for (int z = 0; z<4; z++){
            if(board[rowLoc+z+1][colStart-1][4]){
                return false;
            }
        }
    }

    else if(direction == 2){
        for (int z = 0; z<4; z++){
            if(board[rowLoc+z+1][colEnd-1][4]){
                return false;
            }
        }
    }

    //checks rotation

    return true;
}

//eliminating lines
void shift_board_down(int (*board)[COLS][5], int row_index){

    //the row below will take the values of the row above
    //row index is so that only the rows above the line that was completed are shifted
    for (int i =ROWS-1-row_index; i>0; i--){
        for (int j = 0; j<COLS; j++){
            for (int k = 0; k<5; k++){
                board[i][j][k] = board[i-1][j][k];
            }
        }
    }

    //the first row will be emptied
    for (int j = 0; j<COLS-1; j++){
        board[0][j][0]=0;
        board[0][j][1]=0;
        board[0][j][2]=0;
        board[0][j][3]=0;
        board[0][j][4]=0;
    }

    //the color components will also need to be moved down
    //add that later, for now it'll be 0,0,0 if you try to draw it which is black
}

//checking if lines are full
int line_full_check(int (*board)[COLS][5]){
    for (int j=ROWS-1; j>=0; j--){

        int row_element=0;
        int count = 0;

        for (int i= 0; i<COLS;i++){
            row_element = board[j][i][4];  //k the color var will be ignored
            if(row_element){
                count++;
            }
        }
        
        if (count == COLS){
            shift_board_down(board, ROWS-1-j);
            //append score after this loop to check if other lines were also eliminated
            //append a line counter that will take values up to 4 to record the bonus(?)
            //i think 4 lines = 1200 points
            //*score +=400
        }
    }
    //return score;
}

//score + misc updates
void draw_ui_elements(); 

//draws a grid
void draw_grid(); 

//clears rows when a row is full from bottom to top
void clear_rows(); 

//draws the next piece on the sidebar
void draw_next_piece(SDL_Renderer *renderer, int next_pieces[6]){
    //get_next_piece();
    for (int k = 0; k<6; k++){
        for (int i = 0; i<4; i++){
            for (int j = 0; j<4; j++){
                if ((*piece_shape[next_pieces[k]])[i][j]){
                    SDL_Rect rectNextPiece = {
                        //+8 is the x padding from the bg
                        //+4 is the x padding from the grid
                        .x = board_starting_x + COL_SIZE*10 + 4 + (j*COL_NEXT) + 8,
                        .y = board_starting_y + ROW_SIZE*2 +i*ROW_NEXT + k*ROW_NEXT*4,    // l*nextrow*4 draws next piece after 4 rows
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
    //below can be used to debug frame rate issues
    //SDL_RenderPresent(renderer);
}

//draws the porjection of where the piece may land
void draw_piece_projection(); 

//wasd, c, space, esc keys
int get_key_input(); 

//check time to increase the drop speed of the pieces
int difficulty(); 

bool main_menu(SDL_Renderer *renderer, SDL_Window *screen){

    //surfaces for the image to be displayed on
    SDL_Surface *imageSurface = NULL;  //image surface
    SDL_Surface *windowSurface = NULL; //canvas to display images on

    windowSurface = SDL_GetWindowSurface(screen);

    //load png
    imageSurface = IMG_Load("Images/menu_logo.png");
    if(imageSurface == NULL){
        printf("Image Error: '%s'\n", SDL_GetError());
    }

    bool running = true;
    SDL_Event event_running;
    while (running) {
        while(SDL_PollEvent(&event_running)) {
            if(event_running.type == SDL_QUIT){
                    //exits inits
                    SDL_FreeSurface(imageSurface);
                    SDL_FreeSurface(windowSurface);
                    imageSurface = NULL;
                    windowSurface = NULL;
                    return false;
            }
            else if(event_running.type == SDL_KEYDOWN){
                if (event_running.key.keysym.sym == SDLK_KP_ENTER
                    || event_running.key.keysym.sym ==SDLK_RETURN){
                    //continues to gameplay
                    SDL_FreeSurface(imageSurface);
                    SDL_FreeSurface(windowSurface);
                    imageSurface = NULL;
                    windowSurface = NULL;
                    return true;
                }
            }
        }

                
        /* //bg doesn't work currently
        SDL_SetRenderDrawColor(renderer, 0,120,120,255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);*/

        //blit image does work
        SDL_BlitSurface( imageSurface, NULL, windowSurface, NULL );
        //refreshes screen
        SDL_UpdateWindowSurface(screen);



    }
}

void draw(SDL_Renderer *renderer, Piece *P1, int next_pieces[6], int (*board)[COLS][5])
{
    //background color
    //changes the color of the pallete
    SDL_SetRenderDrawColor(renderer, 0,120,120,255);
    //flushes the screen with whatever colour's selected
    SDL_RenderClear(renderer);

    //creates the grid
    SDL_SetRenderDrawColor(renderer, 10,10,10,255);
    SDL_Rect gridRect = {
        .x = board_starting_x,
        .y = board_starting_y,
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
            if ((P1->shape)[i][j]){ //replace this with the piece shape from get_piece_props
                SDL_Rect rectPiece = {
                    .x = P1->x + (j*COL_SIZE),
                    .y = P1->y + i*ROW_SIZE,
                    .w = COL_SIZE,
                    .h = ROW_SIZE
                };

                //fetching the color of the pieces
                int cur_piece_R = P1->shape[4][0];
                int cur_piece_G = P1->shape[4][1];
                int cur_piece_B = P1->shape[4][2];
                int cur_piece_A = P1->shape[4][3]; //always a const, not needed
                SDL_SetRenderDrawColor(renderer, cur_piece_R, cur_piece_G, cur_piece_B, cur_piece_A);
                SDL_RenderFillRect(renderer, &rectPiece);
            }
        }
    }

    //check if the piece is soft dropped
    piece_board_collision_check(P1, board, next_pieces);

    //check if lines are made and then eliminate them
    line_full_check(board);

    //draw pieces on the board
    //0-3 elements are colors, 4 is occupancy
    for (int i=0; i<ROWS;i++){
        for (int j=0;j<COLS;j++){
            //printf("%d",*board[i][j]); debug
            if((board[i][j][4])){
                SDL_Rect rectPiece = {
                    .x = board_starting_x + (j*COL_SIZE),
                    .y = board_starting_y + i*ROW_SIZE,
                    .w = COL_SIZE,
                    .h = ROW_SIZE
                };

                //easier to understand if you add them to vars
                //we fetch the colors from the 3rd dimension of the array
                int b_r = board[i][j][0];
                int b_g = board[i][j][1];
                int b_b = board[i][j][2];
                int b_a = board[i][j][3];
                SDL_SetRenderDrawColor(renderer, b_r, b_g, b_b, b_a);
                SDL_RenderFillRect(renderer, &rectPiece);
            }
        }
    }   

     //check if the piece is soft dropped
    piece_board_collision_check(P1, board, next_pieces);

    //check if lines are made and then eliminate them
    line_full_check(board);

    //vertical grid lines
    for (int i=0; i<((COLS+1)*COL_SIZE); i+=COL_SIZE) {
        //creating the grid lines
        SDL_SetRenderDrawColor(renderer, 45,45,45,255);
        SDL_Rect gridLines = {
            .x = board_starting_x + i,
            .y = board_starting_y,
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
            .x = board_starting_x,
            .y = (board_starting_y) + i,
            .w = COLS*COL_SIZE,
            .h = 2
        };
        //drawing them
        SDL_RenderFillRect(renderer, &gridLines);
    }

    //creates the bg for the next pieces
    SDL_SetRenderDrawColor(renderer, 10,10,10,255);
    SDL_Rect gridNextRect = {
        .x = board_starting_x + COL_SIZE*10 + 4,
        .y = board_starting_y + ROW_SIZE*1,
        .w = 5*COL_NEXT,
        .h = 25*ROW_NEXT
    };
    SDL_RenderFillRect(renderer, &gridNextRect);

    //to draw the next 6 pieces
    draw_next_piece(renderer, next_pieces);

    //outlines for the next pieces
    //vertical outlines
    for (int i=0; i<5*COL_NEXT; i+=COL_NEXT) {
        //creating the grid lines
        SDL_SetRenderDrawColor(renderer, 10,10,10,255);
        SDL_Rect gridLines = {
            //+8 is the x padding from the bg
            //+4 is the x padding from the grid
            .x = board_starting_x + COL_SIZE*10 + 4 + i + 8, 
            .y = board_starting_y+ ROW_SIZE*1,
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
            .x = board_starting_x + COL_SIZE*10 + 4,
            .y = (board_starting_y) + i + ROW_SIZE*1,
            .w = 5*COL_NEXT,
            .h = 1
        };
        //drawing them
        SDL_RenderFillRect(renderer, &gridLines);
    }

    //updates the screen from the backbuffer
    SDL_RenderPresent(renderer);

}
void gameplay(SDL_Renderer *renderer, int (*board)[COLS][5]) {

    Piece  P1;
    //we get a random shape for our piece along with x,y on the board
    get_piece_props(&P1, -1);

    int next_pieces[6], next_id;

    for (int i = 0; i<6; i++){
        next_id = rand() %7; //rand shape id
        next_pieces[i] = next_id;
    }

    //increase drop_speed wrt time
    int drop_speed = 500;
    long int drop = SDL_GetTicks();
    
    //same loop from board_check
    bool exit_check = false;
    int last_row = 16;

    //timer
    bool timer_three = false;
    bool timer_two = false;
    bool timer_one = false;
    long int timer_time = SDL_GetTicks();

    //collision
    bool possible = false;

    int loop1, loop2;
    for(loop1=3; loop1>=0; loop1--){
        for (loop2=0; loop2<4; loop2++){
            if(P1.shape[loop1][loop2]){
                    last_row += (3-loop1);
                    exit_check = true;
                    break;
            }
            if(exit_check) break;
        }
        if(exit_check) break;
    }

    int lastRow2 = board_starting_y + (last_row-loop1+1)*ROW_SIZE;

    //exit check
    bool running = true;
    SDL_Event eve;
    while (running) {

        //3,2,1
        if (!timer_one){
            if( (SDL_GetTicks() > (timer_time + 1000) ) && !timer_three){
                timer_three = true;
                printf("3\n");
            }
            if( (SDL_GetTicks() > (timer_time + 2000) ) && !timer_two){
                timer_two = true;
                printf("2\n");
            }
            if(SDL_GetTicks() > (timer_time + 5000) ){
                timer_one = true;
                printf("1\n");
                //clear event list
                while (SDL_PollEvent(&eve));
            }
        }

        //we restrict the piece from moving until timer runs out
        
        if(SDL_PollEvent(&eve)) {
            if(eve.type == SDL_QUIT){
                    running = false;
                    return;
            }

            //input check
            if(eve.type == SDL_KEYDOWN){
                if(timer_one){
                    switch(eve.key.keysym.sym){
                        case SDLK_LEFT:
                                possible = sideway_collision_check(&P1, 1, board);
                                //move left
                                if (possible){
                                    P1.x -= COL_SIZE;
                                }
                                possible = false;
                            break;

                        case SDLK_RIGHT:
                                possible = sideway_collision_check(&P1, 2, board);
                                //move right
                                if (possible){
                                    P1.x += COL_SIZE;
                                }
                                possible = false;
                        break;

                        case SDLK_DOWN:
                                //to soft drop the piece
                                if (P1.y <lastRow2-1){
                                    P1.y+=ROW_SIZE;
                                }
                        break;

                        case SDLK_UP:
                                //to rotate the piece
                                //possible = sideway_collision_check();
                                rotate_piece(&P1);
                        break;
                    }
                }
            }
        }
    
        draw(renderer, &P1, next_pieces, board);

        // we get the time and add 500ms to it, after which we check
        // if 500ms has passed. then we drop the piece
        if(timer_one){
            if (SDL_GetTicks() > drop){
                if (P1.y <lastRow2-1){
                    P1.y+=ROW_SIZE;
                }
            drop = SDL_GetTicks() + drop_speed;
            }
        }
        
        //loop continues
    }

}

void gameover();


int main(int argc, char* args[]) {

    //new seed for rand, should only be called once
    // will consider system time as the seed
    srand(time(NULL));

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
    Mix_Music *bgsound = Mix_LoadMUS("Music/Tetris99.mp3");
    if (bgsound == NULL) {
        printf("Mix_LoadMUS: %s\n", Mix_GetError());
    }
    //Mix_Chunk *clear = Mix_LoadWAV("clear.wav"); //sound effects

    //create the window
    SDL_Window *screen = SDL_CreateWindow(
        "TETRIS", 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        SCREEN_WIDTH, SCREEN_HEIGHT, 
        0
    );
    // add | SDL_WINDOW_FULLSCREEN to flags on launch

    //checks if the window was created
    if (screen == NULL) {
        printf("Failed to create the window: %s\n", SDL_GetError());
        return 1;
    }
    
    //initialize the image
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
        printf("Image init error: %s\n", IMG_GetError());
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
    bool running = main_menu(renderer, screen);

    //create the board with 0 values
    int board[ROWS][COLS][5] = {0};

    //gameplay begins
    //we send the board's pointer to the function
    if (running){
        gameplay(renderer, board);
    }
    
    //exit
    Mix_FreeMusic(bgsound);
    //Mix_FreeChunk(clear);
    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(screen);
    SDL_Quit();

    return 0;
}
/*
bug 1: 
    when you enter gameplay, the game lags a bit and because of this, the piece may fall through
    the board and glitch the game. there needs to be a counter that counts to 3 before game starts to 
    avoid this and give the game time to load
bug 2: 
    random will glitch out and give the same piece many times in a row
bug 3: 
    rather hard to replicate, happens more with the O piece
    sometimes pieces just glitch through the bottom of the board
        happens if you hold (down arrow key)
    i think it happens due to P1.y drop. the piece would drop before it could be checked against the board
bug 4: 
    if you don't wait for everything to initialize and press a key, it will mess with the last row
    and won't allow the piece to drop all the way. i guess it needs the 3,2,1 go to fix it
    it's the same as bug 1
    try making renderer and window/screen global

bug 5: 
    i don't know if the above bugs except 2 are still there or not, but there's a bug where if you hold
    a key down and if you hold another key
    down and let go of the key you were previously holding, the pieces will not hit the bottom of the board
    and will just dangle one row above. rotating the piece if it's >3 cells will place it, but the
    subsequent pieces will still continue to dangle.
*/