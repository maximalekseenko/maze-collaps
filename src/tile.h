#pragma once


#include <string>


struct Tile {
    bool isObstacle = false;
    std::string charVisible = "A";
    std::string charDiscovered = "A";
    std::string charUndiscovered = ".";
};