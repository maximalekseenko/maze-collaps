#include "tile.h"
#include "sector.h"



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


// ---------- SECTORS ---------- //

struct SectorNone : Sector{
    SectorNone(){
        Sector::exit_up    = 1;
        Sector::exit_left  = 1;
        Sector::exit_down  = 1;
        Sector::exit_right = 1;

        Sector::tileMap = {
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
        };
    }
};

struct SectorRoomWColumns : Sector{
    SectorRoomWColumns(){
        Sector::exit_up    = 1;
        Sector::exit_left  = 2;
        Sector::exit_down  = 1;
        Sector::exit_right = 2;

        Sector::tileMap = {
            0, 0, 0, 0, 0, 0, 0,
            0, 1, 1, 1, 1, 1, 0,
            0, 1, 0, 1, 0, 1, 0,
            1, 1, 1, 1, 1, 1, 1,
            0, 1, 0, 1, 0, 1, 0,
            0, 1, 1, 1, 1, 1, 0,
            0, 0, 0, 0, 0, 0, 0,
        };
    }
};

struct SectorMazeCorner : Sector{
    SectorMazeCorner(){
        Sector::exit_up    = 2;
        Sector::exit_left  = 1;
        Sector::exit_down  = 1;
        Sector::exit_right = 2;

        Sector::tileMap = {
            0, 0, 0, 1, 0, 0, 0,
            0, 0, 0, 1, 1, 1, 0,
            0, 0, 0, 1, 0, 1, 0,
            0, 1, 1, 1, 1, 1, 1,
            0, 1, 0, 1, 0, 0, 0,
            0, 1, 1, 1, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
        };
    }
};