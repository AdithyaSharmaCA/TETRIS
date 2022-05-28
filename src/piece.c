#include "piece.h"

void getPieceProps(Piece *P1, const int check) {
    //check var here checks if it's the first piece or not
    //if it is the next piece after being placed, then that will be the id
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


int nextPieceShift(Piece *P1, int next_pieces[6]){
    //when we add the piece to the grid after collision or when we hold the piece
    //or when we hard drop the piece,
    //collision_check(&P1, next_pieces);

    //P1 is already a pointer, so i don't think &P1 is necessary
    getPieceProps(P1, next_pieces[0]);

    for (int i = 0; i < (6-1); i++){
        next_pieces[i] = next_pieces[i+1];
    }
    int next_id = rand() %7; //rand shape id
    next_pieces[5] = next_id;

    //we move all the elements by 1 index and then 
        //fill the last element with a random id
}


void rotatePiece(Piece *P1, const int direction){

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

    //rotate left
    if(direction == 2){
        for (int i =0; i<n; i++){
            for (int j =0; j<n; j++){
                P1->shape[n-j-1][i] = temp[i][j]; //anti clockwise
            }
        }
        return;
    }

    //rotate right
    if(direction == 1){
        for (int i =0; i<n; i++){
            for (int j =0; j<n; j++){
                P1->shape[i][j] = temp[n-j-1][i]; //clockwise
            }
        }
        return;
    }    
}


//check time to increase the drop speed of the pieces
int difficulty(){
    
}