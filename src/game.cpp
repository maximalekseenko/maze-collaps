#include "game.h"

#include <iostream>
#include <random>
#include <vector>
#include <ctime>


Game::Game(int __dim){
    this->sec_dim = __dim;
    this->map_dim = __dim * SECTOR_DIM;
    this->Tiles = new Tile[this->map_dim * this->map_dim];

    this->Gen();
}

void Game::Gen(){
    // Random
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution distrib(0, 255);

    // find exit and entrance points on sector map
    int exitPoint[2]     = {-1, -1};
    int entrancePoint[2] = {-1, -1};
    while (exitPoint[0] == entrancePoint[0] && exitPoint[1] == entrancePoint[1]){
        exitPoint[0] = distrib(gen) % this->sec_dim;
        exitPoint[1] = distrib(gen) % this->sec_dim;

        entrancePoint[0] = distrib(gen) % this->sec_dim;
        entrancePoint[1] = distrib(gen) % this->sec_dim;
    }

    // sector entrance matrix
    // where each point is int[up, left, down, right] entrance
    // type specified in EXITTYPES.md
    int* sectorEntrances[this->sec_dim * this->sec_dim];
    for (int _i = 0; _i < this->sec_dim * this->sec_dim; _i ++){
        sectorEntrances[_i] = new int[4];
        sectorEntrances[_i][0] = -1;
        sectorEntrances[_i][1] = -1;
        sectorEntrances[_i][2] = -1;
        sectorEntrances[_i][3] = -1;
    }

    // make path by moving current point randomly
    // from entrance fo exit
    // with no repeats
    int currentPoint[2] = {entrancePoint[0], entrancePoint[1]};
    while (currentPoint[0] != exitPoint[0] || currentPoint[1] != exitPoint[1]){
        std::cout << (currentPoint[0] != exitPoint[0] || currentPoint[1] != exitPoint[1]) << " " << currentPoint[0] << " " <<currentPoint[1] << std::endl;
        switch (distrib(gen) % 4) {
                
            case 0: // up
                // cant move
                if (currentPoint[1] == this->sec_dim - 1) continue;
                // make entrance
                sectorEntrances[currentPoint[0] + currentPoint[1] * this->sec_dim][0] = 0;
                // move
                currentPoint[1] += 1;
                break;

            case 1: // left
                // cant move
                if (currentPoint[0] == this->sec_dim - 1) continue;
                // make entrance
                sectorEntrances[currentPoint[0] + currentPoint[1] * this->sec_dim][2] = 0;
                // move
                currentPoint[0] += 1;
                break;

            case 2: // down
                // cant move
                if (currentPoint[1] == 0) continue;
                // make entrance
                sectorEntrances[currentPoint[0] + currentPoint[1] * this->sec_dim][3] = 0;
                // move
                currentPoint[1] -= 1;
                break;

            case 3: // right
                // cant move
                if (currentPoint[0] == 0) continue;
                // make entrance
                sectorEntrances[currentPoint[0] + currentPoint[1] * this->sec_dim][3] = 0;
                // move
                currentPoint[0] -= 1;
                break;

            case 4: // to exit horizontaly
                break;

            case 5: // to exit verticaly
                break;
        }
    }

    // /DELETE
    for (int _x = 0; _x < this->sec_dim; _x ++){
        for (int _y = 0; _y < this->sec_dim; _y ++){
            // std::cout << sectorEntrances[_x + _y * this->sec_dim][0] 
            //           << sectorEntrances[_x + _y * this->sec_dim][1]
            //           << sectorEntrances[_x + _y * this->sec_dim][2]
            //           << sectorEntrances[_x + _y * this->sec_dim][3];
            if (_x == entrancePoint[0] && _y == entrancePoint[1])
                std::cout << "A";
            else if (_x == exitPoint[0] && _y == exitPoint[1])
                std::cout << "B";
            else if (
                    sectorEntrances[_x + _y * this->sec_dim][0] == -1 &&
                    sectorEntrances[_x + _y * this->sec_dim][1] == -1 &&
                    sectorEntrances[_x + _y * this->sec_dim][2] == -1 &&
                    sectorEntrances[_x + _y * this->sec_dim][3] == -1)
                std::cout << "#";
            else std::cout << ".";
        }
        std::cout << std::endl;
    }
    // \DELETE

    // clean storage
    for (auto _sectorEntrance : sectorEntrances) delete _sectorEntrance;
}




void Game::PrintMap(){
    for (int _x = 0; _x < this->map_dim; _x ++){
        for (int _y = 0; _y < this->map_dim; _y ++){
            std::cout << Tiles[_x + _y * this->map_dim].charVisible;
        }
        std::cout << std::endl;
    }
}


void Game::MakeTile(int __x, int __y, int __id){
    this->Tiles[__x + __y * this->map_dim] = database::GetTile(__id);
}


void Game::MovePoint(int& point, int direction){
    // switch (direction) {
    //     case 0: // up
    //         if (point[1] == this->)
    //         break;
    //     case 1: // left
    //         break;
    //     case 2: // down
    //         break;
    //     case 3: // right
    //         break;
    //     default: throw; break;
    // }
}