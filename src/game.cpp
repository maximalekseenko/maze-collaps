#include "game.h"

#include <iostream>


Game::Game(int __dim){
    this->dim = __dim;
    this->Tiles = new Tile*[__dim * __dim];

    for (int _i = 0; _i < this->dim * this->dim; _i ++){
        Tiles[_i] = new Tile();
    }
}


void Game::PrintMap(){
    for (int _x = 0; _x < this->dim; _x ++){
        for (int _y = 0; _y < this->dim; _y ++){
            std::cout << Tiles[_x + _y * this->dim]->charVisible;
        }
        std::cout << std::endl;
    }
}


void Game::MakeTile(int __x, int __y, int __id){
    *(this->Tiles[__x + __y * this->dim]) = database::GetTile(__id);
}