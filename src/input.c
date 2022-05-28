#include "input.h"

//get key input separately so the piece can move multi directionally
int getKeyInput(KeyInputs *input){
    
    SDL_Event eve;
    while(SDL_PollEvent(&eve)) {
        if(eve.type == SDL_QUIT){
            return 1;
        }

        if(eve.type == SDL_KEYDOWN){
            switch(eve.key.keysym.sym){
                case SDLK_LEFT:
                    input -> left = true;
                break;

                case SDLK_RIGHT:
                    input -> right = true;
                break;

                case SDLK_DOWN:
                    input -> soft_drop = true;
                break;

                case SDLK_SPACE:
                    input -> hard_drop = true;
                break;

                case SDLK_UP:
                    input -> rotate_r = true;
                break;

                case SDLK_LSHIFT:
                    input -> rotate_l = true;
                break;

                case SDLK_c:
                    input -> hold = true;
                break;

                case SDLK_ESCAPE:
                    input -> menu = true;
                break;
            }
        }

        if(eve.type == SDL_KEYUP){
            switch(eve.key.keysym.sym){
                case SDLK_LEFT:
                    input -> left = false;
                break;

                case SDLK_RIGHT:
                    input -> right = false;
                break;

                case SDLK_DOWN:
                    input -> soft_drop = false;
                break;

                case SDLK_SPACE:
                    input -> hard_drop = false;
                break;

                case SDLK_UP:
                    input -> rotate_r = false;
                break;

                case SDLK_LSHIFT:
                    input -> rotate_l = false;
                break;

                case SDLK_c:
                    input -> hold = false;
                break;

                case SDLK_ESCAPE:
                    input -> menu = false;
                break;
            }
        }
    }
    return 0;
}