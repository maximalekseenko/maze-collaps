#pragma once


#include <vector>


#define SECTOR_DIM 7


struct Sector{
    int GetExit(int __dir){
        switch ((__dir + this->rotation) % 4) {
        case 0: return this->exit_up;
        case 1: return this->exit_left;
        case 2: return this->exit_down;
        case 3: return this->exit_right;
        default: return -1;
        }
    }

    int exit_up = 0;
    int exit_left = 0;
    int exit_down = 0;
    int exit_right = 0;

    int rotation = 0;

    std::vector<int> tileMap = {
        0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0,
    };
};