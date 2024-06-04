#include "tile.h"

Tile::Tile() : Tile(0) {}
Tile::Tile(int __type)
{
    this->type = __type;
}
Tile::~Tile(){}