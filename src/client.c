#include "client.h"

int clientSideThread(){

    IPaddress ip;

    SDLNet_ResolveHost(&ip, "127.0.0.1", 1234);

    char recvLines[1];
    //char sendLines[1];

    client = SDLNet_TCP_Open(&ip);


    while(true){

        SDL_LockMutex(mut);

        //server = SDLNet_TCP_Accept(client);
        
        

        SDLNet_TCP_Recv(client, recvLines, 1);
        
        int temp = atoi(recvLines);
        linesToAdd +=temp;

        printf("Recieved %d lines from server\n", linesToAdd);

        //SDLNet_TCP_Close(server);

        //linesCompleted = 0; 
        
        SDL_UnlockMutex(mut);

    }

    return 0;
}