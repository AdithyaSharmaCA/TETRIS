#ifndef INIT_H
#define INIT_H

#include "defs.h"

//reserve globals
extern SDL_Window *screen;
extern SDL_Renderer *renderer;
extern Mix_Music *bgsound;

void initializeSdlElements();

#endif