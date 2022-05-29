#include "defs.h"

const int board_starting_x = (SCREEN_WIDTH / 2 - COLS*20);
const int board_starting_y = COLS*5;


//the last row is the color component
const int shape_i[5][4] = {
    {0,0,0,0},
    {0,0,0,0},
    {1,1,1,1},
    {0,0,0,0},
    //{0,255,255,255}
    {153,217,234,255}
};
const int shape_j[5][4] = {
    {0,0,0,0},
    {1,0,0,0},
    {1,1,1,0},
    {0,0,0,0},
    //{0,0,255,255}
    {0,162,232,255}
};
const int shape_l[5][4] = {
    {0,0,1,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
    //{255,127,0,255}
    {255,127,39,255}
};
const int shape_o[5][4] = {
    {0,0,0,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,0,0,0},
    //{255,255,0,255}
    {255,245,55,255}
};
const int shape_s[5][4] = {
    {0,1,1,0},
    {1,1,0,0},
    {0,0,0,0},
    {0,0,0,0},
    //{0,255,0,255}
    {34,177,76,255}
};
const int shape_t[5][4] = {
    {0,1,0,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
    //{128,0,128,255}
    {163,73,164,255}
};
const int shape_z[5][4] = {
    {1,1,0,0},
    {0,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
    //{255,0,0,255}
    {237,28,36,255}
};

//an array of pointers, so it stores addresses
const int (*piece_shape[7])[5][4] = {
    &shape_i,&shape_j,&shape_l,&shape_o,&shape_s,&shape_t,&shape_z
};