#include "draw.h"

void drawGridBackground(){

    SDL_SetRenderDrawColor(renderer, 10,10,10,255);
    SDL_Rect gridRect = {
        .x = board_starting_x,
        .y = board_starting_y,
        .w = COLS*COL_SIZE,
        .h = ROWS*ROW_SIZE
    };
    SDL_RenderFillRect(renderer, &gridRect);
}


void drawPiece(const Piece *P1){

    for (int i = 0; i<4; i++){
        for (int j = 0; j<4; j++){
            if ((P1->shape)[i][j]){
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
}


void drawBoardPieces(const Piece *P1, const int (*board)[COLS][5]){
    
    //0-3 elements are colors, 4 is occupancy
    for (int i=0; i<ROWS;i++){
        for (int j=0;j<COLS;j++){
            if((board[i][j][4])){
                SDL_Rect rectPiece = {
                    .x = board_starting_x + (j*COL_SIZE),
                    .y = board_starting_y + i*ROW_SIZE,
                    .w = COL_SIZE,
                    .h = ROW_SIZE
                };

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
}


void drawBoardGridLines(){
    //vertical grid lines
    for (int i=0; i<((COLS+1)*COL_SIZE); i+=COL_SIZE) {
        //creating the grid lines
        //SDL_SetRenderDrawColor(renderer, 45,45,45,255);
        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
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
        //SDL_SetRenderDrawColor(renderer, 45,45,45,255);
        SDL_Rect gridLines = {
            .x = board_starting_x,
            .y = (board_starting_y) + i,
            .w = COLS*COL_SIZE,
            .h = 2
        };
        //drawing them
        SDL_RenderFillRect(renderer, &gridLines);
    }
}


void drawNextPiece(const int next_pieces[6]){
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
}


void drawNextGrid(const int next_pieces[6]){
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
    drawNextPiece(next_pieces);

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
}

void drawGarbageGrid(){

    //background
    SDL_SetRenderDrawColor(renderer, 10,10,10,255);
    SDL_Rect gridRect = {
        .x = board_starting_x - COL_SIZE - 6,
        .y = board_starting_y,
        .w = 1*COL_SIZE,
        .h = ROWS*ROW_SIZE
    };
    SDL_RenderFillRect(renderer, &gridRect);

    //vertical
    for (int i=0; i<2*COL_SIZE; i+=COL_SIZE) {
        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_Rect gridLines = {
            //-6 is the x padding from the bg
            .x = board_starting_x - i - 6,
            .y = board_starting_y,
            .w = 2,
            .h = ROWS*ROW_SIZE
        };
        SDL_RenderFillRect(renderer, &gridLines);
    }
    //horizontal
    for (int i=0; i<((ROWS+1)*ROW_SIZE); i+=ROW_SIZE) {
        SDL_Rect gridLines = {
            .x = board_starting_x - COL_SIZE - 6,
            .y = (board_starting_y) + i,
            .w = COLS*COL_SIZE,
            .h = 2
        };
        SDL_RenderFillRect(renderer, &gridLines);
    }
}

void drawGarbageCells(int garbage[ROWS]){
    for (int i = 0, k =0; i<((ROWS)*ROW_SIZE); k++, i+=ROW_SIZE){
        if ((garbage)[k]){
            SDL_Rect rectPiece = {
                .x = board_starting_x - COL_SIZE - 6,
                .y = (board_starting_y) + i,
                .w = COL_SIZE,
                .h = ROW_SIZE
            };

            SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255);
            SDL_RenderFillRect(renderer, &rectPiece);
        }
    }
}

//score + misc updates
void drawUiElements(const long long int *score){

} 

//draws the projection of where the piece may land
void drawPieceProjection(){

}


void drawEverything(Piece *P1, int next_pieces[6], int (*board)[COLS][5], long long int *score, int *garbage)
{
    //Background Color{
    //SDL_SetRenderDrawColor(renderer, 0,120,120,255);
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderClear(renderer);
    //}

    drawGridBackground();

    //we will display the pieces here so that grid lines will overlap the pieces

    //draw the piece
    drawPiece(P1);

    //observation haki
    drawPieceProjection();

    //draw pieces that are in the board array
    drawBoardPieces(P1, board);

    //draw board grid lines
    drawBoardGridLines();

    //draw next grid sidebar
    drawNextGrid(next_pieces);

    //garbage counter
    drawGarbageGrid();

    //garbage cells
    drawGarbageCells(&(*garbage));

    //score + misc displays
    drawUiElements(&(*score));

    //updates the screen from the backbuffer
    SDL_RenderPresent(renderer);

}
