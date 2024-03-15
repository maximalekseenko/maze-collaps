#include "game.h"



#include <tuple>
#include <iostream>
#include <string>
#include <map>
#include <vector>

// #include "userInterface.h"
#include "log.h"



Game Game::game = Game();



Game::Game(){
    this->current_map = new Map("data/theentrance.png");
}

Game::~Game(){
    delete this->current_map;
}


void Game::Run()
{
    for (int y = 0; y < current_map->MX; y ++) {
        std::string line = "";
        for (int x = 0; x < current_map->MX; x ++)
            line += current_map->Get(current_map->I(x,y)) == Map::TILE::WALL ? "█" : ".";
        Log::Out(line);
    }
    
    while (true)
    {
        // turns
        // for (auto& entity : this->entities)
        //     entity.logic.Turn(&entity);

        // // loss
        // for (auto& enemy : Enemy::enemies)
        //     if (enemy.position == Player::player.position) 
        //         throw std::runtime_error("ERROR: GAME LOstty sane ^JST");
                
        // update
        // UserInterface::Update(current_map);

        // victory
        // if (Enemy::enemies.size() == 0) throw std::runtime_error("ERROR: VICTORY");

    }
}



void Game::LoadMap(std::string __mapFileName) 
{
    delete this->current_map;
    this->current_map = new Map(__mapFileName);
    this->InitializeMap(this->current_map);
}


  
void Game::InitializeMap(Map *__map)
{
    this->ClearMap();
    this->entities.clear();
    for (int i = 0; i < current_map->MI; i ++)
    {
        // if spawner
        // if (current_map->Get(i) == current_map->TILE::SPAWNER)
        // {
        //     // spawn player
        //     if (this->entities.size() == 0) 
        //     {
        //         this->entities.push_back(En(i));
        //     }
        //     // spawn enemy
        //     else
        //     {
        //         Enemy::Create(i);
        //     }
        // }
    }
}


void Game::ClearMap() {

}