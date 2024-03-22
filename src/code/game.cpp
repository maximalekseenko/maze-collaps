#include "game.h"



#include <tuple>
#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "lib/random.h"
#include "entity/player.h"
#include "entity/enemy.h"
#include "lib/log.h"
#include "renderer.h"


Game Game::game = Game();


Game::Game(){
    this->current_map = new Map("data/theentrance.png");
}

Game::~Game(){
    delete this->current_map;

    for (auto _ent : this->entities) delete _ent;
    this->entities.clear();
}


void Game::Run()
{
    for (auto &_ent : this->entities) _ent->Turn();
    Renderer::Render_Game(entities[0]->position);

    for (auto &_ent : this->entities) _ent->Turn();
    Renderer::Render_Map(current_map,36,0,entities[0]->position);
    for (auto _ent : entities) Renderer::renderer.Render_Entity(current_map, _ent, 36, 0, entities[0]->position);
    
    for (auto &_ent : this->entities) _ent->Turn();
    Renderer::Render_Map(current_map,0,20,entities[0]->position);
    for (auto _ent : entities) Renderer::renderer.Render_Entity(current_map, _ent, 0, 20, entities[0]->position);

    for (auto &_ent : this->entities) _ent->Turn();
    Renderer::Render_Map(current_map,36,20,entities[0]->position);
    for (auto _ent : entities) Renderer::renderer.Render_Entity(current_map, _ent, 36, 20, entities[0]->position);

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
                this->entities.push_back(this->MakeEntity(i));
        }
    }
}


void Game::ClearMap() {

}

Entity* Game::MakeEntity(int __pos) {
    return new Enemy(__pos);
}