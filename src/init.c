#include "init.h"

//initialize sdl elements globally
SDL_Window *screen;
SDL_Renderer *renderer;
Mix_Music *bgsound;

void initializeSdlElements(){
    //initialize everything and then a check if it initialized properly
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("Error: SDL failed to initialize: '%s'\n", SDL_GetError());
        exit(1);
    }

    //initialize sdl mixer
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048)==-1) {
        printf("Error Mix_OpenAudio: %s\n", Mix_GetError());
        exit(1);
    }
    
    //load audio files
    //error if music not found or invalid filename
    bgsound = Mix_LoadMUS("Resources/Music/Tetris99.mp3");
    if (bgsound == NULL) {
        printf("Error Mix_LoadMUS: %s\n", Mix_GetError());
        exit(1);
    }
    //Mix_Chunk *clear = Mix_LoadWAV("clear.wav"); //sound effects

    //create the window
    screen = SDL_CreateWindow(
        "TETRIS", 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        SCREEN_WIDTH, SCREEN_HEIGHT, 
        0
    );
    // add | SDL_WINDOW_FULLSCREEN to flags on launch

    //checks if the window was created
    if (screen == NULL) {
        printf("Error: Window failed to initialize %s\n", SDL_GetError());
        //might create memory leak for above creations
        exit(1);
    }

    //initialize the image
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
        printf("Error: Window failed to initialize %s\n", IMG_GetError());
        exit(1);
    }

    //create the renderer
        //index -1 is the default gpu index
        //we use hardware acceleration and vsync
    renderer = SDL_CreateRenderer(screen, -1, 
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    //adjust volume
    Mix_VolumeMusic(MIX_MAX_VOLUME*0);
    //change volume back to 0.3 during release

    //play music
    if(Mix_PlayMusic(bgsound, -1)==-1) {
        printf("Error Mix_PlayMusic: %s\n", Mix_GetError());
        exit(1);
    }

    return;
}
