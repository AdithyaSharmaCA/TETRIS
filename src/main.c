#include "defs.h"
#include "init.h"
#include "input.h"
#include "draw.h"
#include "piece.h"
#include "board.h"
#include "tetris.h"

int main(int argc, char* args[]) {

    //sets seed to be time variant
    srand(time(NULL));
    
    initializeSdlElements();

    bool quitCheck = false;
    bool running = true;

    //loop back to menu
    while (!quitCheck && running){
        //main menu screen
        running = main_menu();

        //create the board with 0 values
        int board[ROWS][COLS][5] = {0};

        //tetris effect begins
        if (running){
            quitCheck = gameplay(board);
        }
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
