#pragma once

int TILE_DIM = 3;

class Tile{
    public:
        char tile_chars[3][3];
        int type_up;
        int type_left;
        int type_right;
        int type_down;
};