#include "tile.h"

namespace database {
    Tile GetTile(int __id);
}




// ---------- TILES ---------- //

struct TileWall : public Tile{
    TileWall(){
        Tile::isObstacle = true;
        Tile::charVisible = "█";
        Tile::charDiscovered = "▓";
    }
};

struct TileFloor : public Tile{
    TileFloor(){
        Tile::charVisible = " ";
        Tile::charDiscovered = "░";
    }
};