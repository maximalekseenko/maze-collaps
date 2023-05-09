#pragma once

#include <set>
#include <string>


struct Tile
{
    std::string symbol;
};


std::set<int, Tile> Tiles({
    {1052688,  Tile(".")},
    {10526880, Tile("█")},
    {1052832,  Tile("◙")},
    {1089552,  Tile("#")},
});