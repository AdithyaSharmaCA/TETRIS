#include "server.h"

int serverSideThread(void *ptr){

    IPaddress ip;

    if (SDLNet_ResolveHost(&ip, NULL, 1234) <0){
        printf("SDLNet_Resolve Host: '%s'\n", SDLNet_GetError());
        exit(1);
    }

    server = SDLNet_TCP_Open(&ip);

    char sendLines[1];
    char recvLines[1];

    while(true){

        client = SDLNet_TCP_Accept(server);
        if(!client){
            printf("Waiting for connection:\n");
            SDL_Delay(100);
        }
        else{
            printf("Connected to client\n");
        }

        while(client){
            
            SDL_LockMutex(mut);

            SDLNet_TCP_Recv(client, recvLines, 1);
            //linesToAdd = atoi(recvLines);
            linesToAdd += (recvLines[0] - '0');
            printf("Recieved %d lines from client\n", linesToAdd);
            //SDLNet_TCP_Close(client);    

            printf("Server completed %d lines\n", linesCompleted);
            sprintf(sendLines, "%d", linesCompleted);
            SDLNet_TCP_Send(client, sendLines, 1);
            //SDLNet_TCP_Close(client);     

            linesCompleted = 0;    

            SDL_UnlockMutex(mut);
        }

        
    }    
    
    return 0;
}
