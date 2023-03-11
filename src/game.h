#include "sector.h"
#include "tile.h"
#include "database.h"


#include <vector>


class Game {
    private:
        void Gen();
    public:
        Game(int __dim = 7);

        void PrintMap();
        void MakeTile(int __x, int __y, int __id);

        int sec_dim;
        int map_dim;
        Tile* Tiles;
        // std::vector<Entity> Entities;
        // std::vector<Tile> Tiles;

        void MovePoint(int& point, int direction);
};