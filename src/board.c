#include "board.h"

//if the piece collides with something on the board or the edges of the board
bool collisionCheck(const int (*board)[COLS][5], const Piece *P1){

    int boardX = ((P1->x - board_starting_x) / ROW_SIZE);
    int boardY = ((P1->y - board_starting_y) / COL_SIZE);

    for (int i = 0; i<4; i++){
        for (int j =0; j<4; j++){
            
            if(P1->shape[i][j]){
                if(boardX + j <0){
                    return true;
                }
                if(boardX + j >= COLS){
                    return true;
                }
                if(boardY + i >=ROWS){
                    return true;
                }
                if(board[boardY + i][boardX + j][4]){
                    return true;
                }
            }

            //(boardY <0) check needed if the piece is spawned 2 rows above

        }
    }
    return false;
}

//eliminating lines
void shiftBoardDown(int (*board)[COLS][5], const int row_index){

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
    for (int j = 0; j<COLS; j++){
        board[0][j][0]=0;
        board[0][j][1]=0;
        board[0][j][2]=0;
        board[0][j][3]=0;
        board[0][j][4]=0;
    }
}


//checking if lines are made
void lineFullCheck(int (*board)[COLS][5], long long int *score, int *linesCleared){

    int level = 0;

    for (int j=ROWS-1; j>=0; j--){

        int rowElement=0;
        int count = 0;

        for (int i= 0; i<COLS;i++){
            rowElement = board[j][i][4];  //k the color var will be ignored
            if(rowElement){
                count++;
            }
        }
        
        if (count == COLS){            
            shiftBoardDown(board, ROWS-1-j);
            *linesCleared += 1;
        }
    }

    //Original NES scoring system
    switch(*linesCleared){
        case 1:
            *score += (40 * (level+1));
            break;
        case 2:
            *score += (100 * (level+1));
            break;
        case 3:
            *score += (300 * (level+1));
            break;
        case 4:
            *score += (1200 * (level+1));
            break;
    }

    return;
}

//add junk lines
void addJunkLines(int (*board)[COLS][5], int voidCell, Piece *P1){

    //the row above will take the values of the row beneath
    for (int i =1; i<ROWS; i++){
        for (int j = 0; j<COLS; j++){
            for (int k = 0; k<5; k++){
                board[i-1][j][k] = board[i][j][k];
            }
        }
    }

    //the last row will be a junk line
    for (int j = 0; j<COLS; j++){
        if(j != voidCell){
            board[ROWS-1][j][0]=127;
            board[ROWS-1][j][1]=127;
            board[ROWS-1][j][2]=127;
            board[ROWS-1][j][3]=255;
            board[ROWS-1][j][4]=1;
        }
    }
    board[ROWS-1][voidCell][0]=0;
    board[ROWS-1][voidCell][1]=0;
    board[ROWS-1][voidCell][2]=0;
    board[ROWS-1][voidCell][3]=0;
    board[ROWS-1][voidCell][4]=0;

    //so that the piece doesn't collide with first added junk line
    while( (collisionCheck(board, P1)) ){
        P1->y -=ROW_SIZE;
    }
}

void addPieceToBoard(int (*board)[COLS][5], Piece* P1){

    int boardX = ((P1->x - board_starting_x) / ROW_SIZE);
    int boardY = ((P1->y - board_starting_y) / COL_SIZE);

    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            if( P1->shape[i][j]){
                board[boardY+ i][boardX+ j][4] = 1;
                //color
                board[boardY+ i][boardX+ j][0] = P1->shape[4][0];
                board[boardY+ i][boardX+ j][1] = P1->shape[4][1];
                board[boardY+ i][boardX+ j][2] = P1->shape[4][2];
                board[boardY+ i][boardX+ j][3] = P1->shape[4][3];
            }
        }
    }
    return;
}


int dropCheck(int (*board)[COLS][5], Piece *P1, long long int *score, int next_pieces[6]){

    int linesCleared2;
    linesCleared2 =0;

    //add lines before any collision checks, might have to change this later because
    int voidCell = rand() %10;


    SDL_LockMutex(mut);

    while(linesToAdd){
        addJunkLines(board, voidCell, P1);
        linesToAdd -= 1;
    }


    P1->y += ROW_SIZE;
    if( ( (collisionCheck(board, P1) ) == false) ){
        //collision not possible
        return 0;
    }
    P1->y -= ROW_SIZE;

    //since it collides, we add it to the board one row above where collision took place
    addPieceToBoard(board, P1);
    
    //check if it completed a line, and if so, eliminate it, ++score;

    //there's the bug where the only alternate lines are cleared,
    //i don't know what's causing this
    //a temp fix is to call the function 3 times
    lineFullCheck(board, &(*score), &(linesCleared2));

    nextPieceShift(P1, next_pieces);

    lineFullCheck(board, &(*score), &(linesCleared2));

    lineFullCheck(board, &(*score), &(linesCleared2));

    if(linesCleared2){
        linesCompleted = linesCleared2;
    }

    //send data
    char sendLines[1];

    printf("Client completed %d lines\n", linesCompleted);
    //sprintf(sendLines, "%d", linesCompleted);
    itoa (linesCompleted, sendLines, 10);
    SDLNet_TCP_Send(client, sendLines, 1);
    linesCompleted = 0; 

    SDL_UnlockMutex(mut);


    //check if the new piece collides with anything, i.e game over
    if( (collisionCheck(board, P1) ) == true){
        return -1;
    }

    return 1;

}
