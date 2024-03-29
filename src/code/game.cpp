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

#include "userinterface/visualcomponent.h"



Map* Game::current_map = new Map("data/theentrance.png");;
std::vector<Entity*> Game::entities;

// TEMP
bool game_run;
class UIButton : public VisualComponent
{
    public:
        int action;
        std::string text;
        UIButton(int __y, std::string _s, int __a)
        : VisualComponent(30, __y, 15, 3, VisualComponent::Layer::BUTTONS) 
        {
            std::lock_guard el_lock(lock);
            text = _s;

            this->AddLine(2,    1, text.c_str(),      Renderer::Color::YELLOW);
            this->AddLine(0,    0, "┌─────────────" , Renderer::Color::WHITE);
            this->AddLine(14,   0,               "┐", Renderer::Color::BRIGHT_BLACK);
            this->AddLine(0,    1, "│"              , Renderer::Color::WHITE);
            this->AddLine(14,   1,               "│", Renderer::Color::BRIGHT_BLACK);
            this->AddLine(0,    2, "└"              , Renderer::Color::WHITE);
            this->AddLine(1,    2,  "─────────────┘", Renderer::Color::BRIGHT_BLACK);
        }
        void OnHover(bool __on) override
        {
            this->AddLine(2,    1, text.c_str(),        __on ? Renderer::Color::BRIGHT_YELLOW : Renderer::Color::YELLOW);
        }
        void OnClick(bool __on) override
        {
            Log::Out("HOV");
            if (action == 0)
                game_run = true;
            if (action == 1)
            {   
                this->text = "USE ^C LOL";
                this->AddLine(2,    1, text.c_str(), Renderer::Color::YELLOW);
            }
        }
};
#define MAPX 1
#define MAPY 1
class UIMap : public VisualComponent
{
    public:
        int action;
        UIMap() : VisualComponent(MAPX, MAPY, 32, 16, VisualComponent::Layer::STATIC) 
        {
            std::lock_guard el_lock(lock);

            for (int _i = 0; _i < Game::current_map->MI; _i ++)
                this->AddLine(
                        Game::current_map->X(_i),
                        Game::current_map->Y(_i),
                        Game::current_map->Get(_i) == Map::TILE::WALL ? "█" : ".",
                        Game::current_map->Get(_i) == Map::TILE::WALL ? Renderer::Color::WHITE : Renderer::Color::BRIGHT_BLACK  
                );

        }
};
class UIBack : public VisualComponent
{
    public:
        int action;
        UIBack() : VisualComponent(MAPX-1, MAPY-1, 34, 18, VisualComponent::Layer::BACKGROUND) 
        { std::lock_guard el_lock(lock);

            this->AddLine(0, 0, "┌────────────────────────────────" , Renderer::Color::WHITE);
            // this->AddLine(33,0,               "", Renderer::Color::BRIGHT_BLACK);
            this->AddLine(0, 1, "│\n│\n│\n│\n│\n│\n│\n│\n│\n│\n│\n│\n│\n│\n│\n│\n└", Renderer::Color::WHITE);
            this->AddLine(33, 0, "┐", Renderer::Color::BRIGHT_BLACK);
            this->AddLine(33, 1, "│", Renderer::Color::BRIGHT_BLACK);
            this->AddLine(33, 2, "│", Renderer::Color::BRIGHT_BLACK);
            this->AddLine(33, 3, "│", Renderer::Color::BRIGHT_BLACK);
            this->AddLine(33, 4, "│", Renderer::Color::BRIGHT_BLACK);
            this->AddLine(33, 5, "│", Renderer::Color::BRIGHT_BLACK);
            this->AddLine(33, 6, "│", Renderer::Color::BRIGHT_BLACK);
            this->AddLine(33, 7, "│", Renderer::Color::BRIGHT_BLACK);
            this->AddLine(33, 8, "│", Renderer::Color::BRIGHT_BLACK);
            this->AddLine(33, 9, "│", Renderer::Color::BRIGHT_BLACK);
            this->AddLine(33, 10, "│", Renderer::Color::BRIGHT_BLACK);
            this->AddLine(33, 11, "│", Renderer::Color::BRIGHT_BLACK);
            this->AddLine(33, 12, "│", Renderer::Color::BRIGHT_BLACK);
            this->AddLine(33, 13, "│", Renderer::Color::BRIGHT_BLACK);
            this->AddLine(33, 14, "│", Renderer::Color::BRIGHT_BLACK);
            this->AddLine(33, 15, "│", Renderer::Color::BRIGHT_BLACK);
            this->AddLine(33, 16, "│", Renderer::Color::BRIGHT_BLACK);
            this->AddLine(1, 17, "────────────────────────────────┘" , Renderer::Color::BRIGHT_BLACK);

            this->AddLine(0, 18, "┌────────────────────────────────" , Renderer::Color::WHITE);
            this->AddLine(33, 18, "┐", Renderer::Color::BRIGHT_BLACK);
            this->AddLine(0, 19, "│\n└" , Renderer::Color::WHITE);
            this->AddLine(33, 19, "│", Renderer::Color::BRIGHT_BLACK);
            this->AddLine(1, 20, "────────────────────────────────┘" , Renderer::Color::BRIGHT_BLACK);

            this->AddLine(1, 19, "♥♥♥" , Renderer::Color::BRIGHT_RED);
            this->AddLine(4, 19, "♥♥♥♥" , Renderer::Color::RED);
        }
};
class UIFoe : public VisualComponent
{
    public:
        int action;
        UIFoe() : VisualComponent(0, 0, 1, 1, VisualComponent::Layer::DYNAMIC) 
        {
            std::lock_guard el_lock(lock);
        }
        void SetEnt(Entity* __ent)
        {
            std::lock_guard el_lock(lock);
            SetXY(
                Game::current_map->X(__ent->position) + MAPX,
                Game::current_map->Y(__ent->position) + MAPY
            );

            this->AddLine(
                    0, 0,
                    __ent->GetVisual(),
                    __ent->id == 0 ? Renderer::Color::GREEN : Renderer::Color::BRIGHT_RED
            );
        }
};



void Game::Run()
{
    {   
        UIButton b1(1, "PLAY", 0);
        UIButton b2(6, "EXIT", 1);
        b1.Activate();
        b2.Activate();
        while (!game_run) {}
    }

    {
        UIBack b;
        UIMap m;
        m.Activate();
        b.Activate();
        UIFoe* ents = new UIFoe[entities.size()];
        for (int i=0; i<entities.size();i++)
        {
            ents[i].SetEnt(entities[i]);
            ents[i].Activate();
        }
        while (game_run) {}

        delete[] ents;
    }

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