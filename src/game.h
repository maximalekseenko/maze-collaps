#include "tile.h"
#include "database.h"


#include <vector>


class Game{
    public:
        Game(int __dim = 7);

        void PrintMap();
        void MakeTile(int __x, int __y, int __id);

        int dim;
        Tile** Tiles;
        // std::vector<Entity> Entities;
        // std::vector<Tile> Tiles;
};