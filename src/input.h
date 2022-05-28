#ifndef INPUT_H
#define INPUT_H

#include "defs.h"

//we check if the keyinput is up or down
typedef struct KeyInputs{
    bool left;       //move piece left , (Left arrow key)
    bool right;      //move piece right, (Right arrow key)
    bool rotate_l;   //rotate anti-clockwise , (Up arrow key)
    bool rotate_r;   //rotate clockwise , (Shift)
    bool soft_drop;  //(Down arrow key)
    bool hard_drop;  //(spacebar)
    bool hold;       //hold piece or swap piece, (C)
    bool menu;       //exit to main menu(only for debugging) (Esc)
}KeyInputs;

int getKeyInput(KeyInputs *);

#endif