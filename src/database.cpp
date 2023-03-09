#include "database.h"


Tile database::GetTile(int __id){
    switch (__id) {
        case 0: return TileWall();
        case 1: return TileFloor();
        default: return Tile();
    }
}