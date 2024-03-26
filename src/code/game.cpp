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



Map* Game::current_map = new Map("data/theentrance.png");;
std::vector<Entity*> Game::entities;

// TEMP
bool game_run;
class Button : public InterfaceElement
{
    public:
        int action;
        Button(int __y, std::string _s, int __a) : InterfaceElement(InterfaceElement::Layer::BUTTONS) 
        {
            action = __a;
            y = __y;
            x = 30;
            w = 15;
            h = 3;
            visualComponent = VisualComponent({
                VisualComponent::VisualComponentRule(2,1,_s.c_str(), Renderer::Color::YELLOW),
                VisualComponent::VisualComponentRule(0, 0, "┌─────────────" , Renderer::Color::WHITE),
                VisualComponent::VisualComponentRule(14,0,               "┐", Renderer::Color::BRIGHT_BLACK),
                VisualComponent::VisualComponentRule(0, 1, "│"              , Renderer::Color::WHITE),
                VisualComponent::VisualComponentRule(14,1,               "│", Renderer::Color::BRIGHT_BLACK),
                VisualComponent::VisualComponentRule(0, 2, "└"              , Renderer::Color::WHITE),
                VisualComponent::VisualComponentRule(1, 2,  "─────────────┘", Renderer::Color::BRIGHT_BLACK),
            });
        }
        void OnHover(bool __on) override
        {
            visualComponent.rules[0].colorF = __on ? Renderer::Color::BRIGHT_YELLOW : Renderer::Color::YELLOW;
        }
        void OnClick(bool __on) override
        {
            Log::Out("CLICK");
            if (action == 0)
                game_run = true;
            if (action == 1)
                visualComponent.rules[0].ch = "USE ^C LOL";
        }
};



void Game::Run()
{
    
    Button* b1 = new Button(1, "PLAY", 0);
    Button* b2 = new Button(6, "EXIT", 1);
    while (!game_run) {}
    {
        std::lock_guard layers_locker(InterfaceElement::layers_lock);
        // delete b1;
        // delete b2;
        InterfaceElement::layers[b1->layer].erase(
            std::remove(
                InterfaceElement::layers[b1->layer].begin(), 
                InterfaceElement::layers[b1->layer].end(), 
                b1), 
            InterfaceElement::layers[b1->layer].end()
        );
    }

    delete b1;

    while (game_run) {}

        // Log::Out("g" + std::to_string(InterfaceElement::layers[InterfaceElement::Layer::BUTTONS].size()));
    // {
        // Renderer::Render_Game(entities[0]->position);
        // for (auto &_ent : this->entities) _ent->Turn();
    // }
}


void Game::LoadMap(std::string __mapFileName) 
{
    delete Game::current_map;
    Game::current_map = new Map(__mapFileName);
    Game::InitializeMap(Game::current_map);
}


  
void Game::InitializeMap(Map *__map)
{
    Game::ClearMap();
    Game::entities.clear();
    for (int i = 0; i < current_map->MI; i ++)
    {
        // if spawner
        if (current_map->Get(i) == current_map->TILE::SPAWNER)
        {
            if (Game::entities.size() == 0)
                Game::entities.push_back(new Player(i));
            else
                Game::entities.push_back(Game::MakeEntity(i));
        }
    }
}


void Game::ClearMap() {

}

Entity* Game::MakeEntity(int __pos) {
    return new Enemy(__pos);
}