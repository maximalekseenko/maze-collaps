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

#include "userinterface/interfaceelement.h"


Game Game::game = Game();


Game::Game(){
    this->current_map = new Map("data/theentrance.png");
}

Game::~Game(){
    delete this->current_map;

    for (auto _ent : this->entities) delete _ent;
    this->entities.clear();
}

// TEMP
class Button : public InterfaceElement
{
    public:
        Button(int __y, std::string _s) : InterfaceElement(InterfaceElement::Layer::BUTTONS) 
        {
            y = __y;
            x = 10;
            w = 10;
            h = 3;
            visualComponent = VisualComponent({
                VisualComponent::VisualComponentRule(0,0,"+------+", Renderer::Color::CYAN),
                VisualComponent::VisualComponentRule(0,1,"|      |", Renderer::Color::CYAN),
                VisualComponent::VisualComponentRule(2,1,_s.c_str(), Renderer::Color::YELLOW),
                VisualComponent::VisualComponentRule(0,2,"+------+", Renderer::Color::CYAN)
            });
        }
};


void Game::Run()
{
    Button b1 = Button(1, "PLAY");
    Button b2 = Button(6, "EXIT");
    while (true)
        Log::Out("g" + std::to_string(InterfaceElement::layers[InterfaceElement::Layer::BUTTONS].size()));
    // {
        // Renderer::Render_Game(entities[0]->position);
        // for (auto &_ent : this->entities) _ent->Turn();
    // }
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