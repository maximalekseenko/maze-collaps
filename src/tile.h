#pragma once

#include <string>

struct Tile {
    bool isObstacle = false;
    std::string charVisible = "*";
    std::string charDiscovered = " ";
    std::string charUndiscovered = ".";
};