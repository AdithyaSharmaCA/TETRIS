#ifndef CLIENT_H
#define CLIENT_H

#include "defs.h"

extern int linesCompleted;
extern int linesToAdd;

extern TCPsocket client;
extern TCPsocket server;
extern SDL_mutex *mut;

int clientSideThread();

#endif