#ifndef SERVER_H
#define SERVER_H

#include "defs.h"

extern TCPsocket server;
extern TCPsocket client;
extern SDL_mutex *mut;

extern int linesCompleted;
extern int linesToAdd;

int serverSideThread(void* ptr);

#endif