#include "game.h"



#include <tuple>
#include <iostream>
#include <string>
#include <map>
#include <vector>

// #include "userInterface.h"
#include "lib/log.h"
#include "renderer.h"


Game Game::game = Game();


Game::Game(){
    this->current_map = new Map("data/theentrance.png");
}

Game::~Game(){
    delete this->current_map;
}


void Game::Run()
{
    Renderer::Render_Game(entities[0].position);

    while (true)
    {
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
        if (current_map->Get(i) == current_map->TILE::SPAWNER)
        {
            this->entities.push_back(Entity(i));
            // // spawn player
            // if (this->entities.size() == 0) 
            // {
            //     this->entities.push_back(En(i));
            // }
            // spawn enemy
            // else
            // {
                // Enemy::Create(i);
            // }
        }
    }
}


void Game::ClearMap() {

}