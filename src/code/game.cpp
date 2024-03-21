#include "game.h"



#include <tuple>
#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "entity/player.h"
#include "lib/log.h"
#include "renderer.h"


Game Game::game = Game();


Game::Game(){
    this->current_map = new Map("data/theentrance.png");
}

Game::~Game(){
    delete this->current_map;
    
    for (auto __ent : this->entities) delete __ent;
    this->entities.clear();
}


void Game::Run()
{
    Renderer::Render_Game(entities[0]->position);
    for (auto __ent : this->entities) __ent->Turn();

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
            if (this->entities.size() == 0)
                this->entities.push_back(new Player(i));
            else
                this->entities.push_back(new Entity(i));
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