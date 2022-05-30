#include "tetris.h"

bool main_menu(){

    //surfaces for the image to be displayed on
    SDL_Surface *imageSurface = NULL;  //image surface
    SDL_Surface *windowSurface = NULL; //canvas to display images on

    windowSurface = SDL_GetWindowSurface(screen);

    //load png
    imageSurface = IMG_Load("Resources/Images/menu_logo.png");
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

void gameover(KeyInputs *Input, long long int *score){
    int time = SDL_GetTicks();
    //exit after 2
    while(true){
        if (SDL_GetTicks() > time + 1000){
            Input -> menu = 1;
            printf("%d\n",*score);
            return;
        }
    }
}


bool gameplay(int (*board)[COLS][5]) {

    Piece  P1;
    //we get a random shape for our piece along with x,y on the board
    getPieceProps(&P1, -1);

    KeyInputs kInput = {0};

    int next_pieces[6], next_id;

    for (int i = 0; i<6; i++){
        next_id = rand() %7; //rand shape id
        next_pieces[i] = next_id;
    }

    //creating garbage column
    int garbage[ROWS] = {0};
    

    //increase drop_speed wrt time
    int drop_speed = 500;//ms
    long long int time_until_next_drop = SDL_GetTicks() + drop_speed;
    int input_delay = 60;
    long long int time_until_next_input = SDL_GetTicks() + input_delay;
    int soft_drop_dist = 0;

    //garbage addition time
    int junkDelay = 2500;
    long long int time_until_next_garbage = SDL_GetTicks() + junkDelay; //inc not needed

    //score
    long long int score = 0;

    //exit check
    bool running = true;
    SDL_Event eve;
    while (running) {
        
        if(getKeyInput(&kInput)){
            return true;
        }

        if(SDL_GetTicks() > time_until_next_input){

            time_until_next_input = SDL_GetTicks() + input_delay;

            if(kInput.left){
                P1.x -=COL_SIZE;
                if(collisionCheck(board, &P1)) {
                    P1.x +=COL_SIZE;
                }
            }

            if(kInput.right){
                //check if the piece after moving collides with anything
                P1.x +=COL_SIZE;
                if(collisionCheck(board, &P1)) {
                    P1.x -=COL_SIZE;
                }
            }        

            if(kInput.rotate_r){
                //check if the piece after rotating collides with anything
                rotatePiece(&P1, 1);
                if(collisionCheck(board, &P1)) {
                    rotatePiece(&P1, 2);
                }   
            }
            //don't rotate anymore
            kInput.rotate_r = 0;

            if(kInput.rotate_l){
                rotatePiece(&P1, 2);
                if(collisionCheck(board, &P1)) {
                    rotatePiece(&P1, 1);
                } 
            }
            //don't rotate anymore
            kInput.rotate_l = 0;

            if(kInput.hold){

            }

            //back to main menu
            if(kInput.menu){
                return false;
            }
        }
        
        if (kInput.soft_drop){
            soft_drop_dist = ( (13*drop_speed)/14 );
        }
        else{
            soft_drop_dist = 0;
        }

        if (SDL_GetTicks() > time_until_next_drop - soft_drop_dist){

            int drop_possible = dropCheck(board, &P1, &score, next_pieces, garbage, junkDelay, &time_until_next_garbage);

            if(drop_possible){
                //increasing drop speed
                if(drop_speed > 60){
                    drop_speed -= 5;
                }
            }
            if(drop_possible == -1){
                while( (collisionCheck(board, &P1)) ){
                    P1.y -=ROW_SIZE;
                }
                drawEverything(&P1, next_pieces, board, &score, garbage);
                gameover(&kInput , &score);
                return false;
            }
            time_until_next_drop = SDL_GetTicks() + drop_speed;
        }

        while (kInput.hard_drop){

            int drop_possible = dropCheck(board, &P1, &score, next_pieces, garbage, junkDelay, &time_until_next_garbage);

            if(drop_possible){
                //increasing drop speed
                if(drop_speed > 60){
                    drop_speed -= 5;
                }
                //exit loop
                kInput.hard_drop = 0;
            }

            if(drop_possible == -1){
                while( (collisionCheck(board, &P1)) ){
                    P1.y -=ROW_SIZE;
                }
                drawEverything(&P1, next_pieces, board, &score, garbage);
                gameover(&kInput , &score);
                return false;
            }
            time_until_next_drop = SDL_GetTicks() + drop_speed;
        }

        drawEverything(&P1, next_pieces, board, &score, garbage);

    }

}
