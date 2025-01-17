#include "game.h"



#include <tuple>
#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "lib/random.h"
#include "entity/player.h"
#include "entity/enemy.h"
#include "utility/log.h"

#include "engine/visualcomponent.h"
#include "engine/engine.h"



Map* Game::current_map = new Map(0, 0, "");
std::vector<Entity*> Game::entities;

// TEMP
bool game_run;
class UIButton : public VisualComponent
{
    public:
        int action;
        std::string text;
        UIButton(int __y, std::string __s, int __a)
        : VisualComponent(30, __y, 15, 3, VisualComponent::Layer::BUTTONS, "", Color::BLACK) 
        {
            std::lock_guard el_lock(lock);
            text = __s;
            action = __a;

            this->AddLine(2,    1, text.c_str(),      Color::YELLOW);
            this->AddLine(0,    0, "┌─────────────" , Color::WHITE);
            this->AddLine(14,   0,               "┐", Color::BRIGHT_BLACK);
            this->AddLine(0,    1, "│"              , Color::WHITE);
            this->AddLine(14,   1,               "│", Color::BRIGHT_BLACK);
            this->AddLine(0,    2, "└"              , Color::WHITE);
            this->AddLine(1,    2,  "─────────────┘", Color::BRIGHT_BLACK);
        }
        bool OnHover(bool __on, int, int) override
        {
            this->AddLine(2,    1, text.c_str(),        __on ? Color::BRIGHT_YELLOW : Color::YELLOW);
            return true;
        }
        bool OnClick(int, int) override
        {
            if (action == 0)
                game_run = true;
            if (action == 1)
            {   
                this->text = "USE ^C LOL";
                this->AddLine(2,    1, text.c_str(), Color::YELLOW);
            }
            return true;
        }
};
#define MAPX 34
#define MAPY 10
int FixMapX(int __x)   { return (Game::current_map->MX * 3/2 + __x - Game::current_map->X(Game::entities[0]->position)) % Game::current_map->MX; }
int UnFixMapX(int __x) { return (Game::current_map->MX * 3/2 + __x + Game::current_map->X(Game::entities[0]->position)) % Game::current_map->MX; }
int FixMapY(int __y)   { return (Game::current_map->MY * 3/2 + __y - Game::current_map->Y(Game::entities[0]->position)) % Game::current_map->MY; }
int UnFixMapY(int __y) { return (Game::current_map->MY * 3/2 + __y + Game::current_map->Y(Game::entities[0]->position)) % Game::current_map->MY; }

int GetPlayerDirX(int __x, int __y) { return __x==0&&__y==0 ? 0 : round(cos(atan2(__y, __x))); }
int GetPlayerDirY(int __x, int __y) { return __x==0&&__y==0 ? 0 : round(sin(atan2(__y, __x))); }


class UIMap : public VisualComponent
{
    public:
        int action;
        UIMap() : VisualComponent(MAPX, MAPY, 32, 16, VisualComponent::Layer::STATIC, "", Color::BLACK) 
        {   std::lock_guard el_lock(lock);

            UpdateWindow();
        }


        void UpdateEntity(Entity* __ent, Color __colorB=Color::BLACK)
        {
            this->AddLine(
                FixMapX(Game::current_map->X(__ent->position)), 
                FixMapY(Game::current_map->Y(__ent->position)),
                __ent->GetVisual(),
                __ent->id == 0 ? Color::GREEN : Color::BRIGHT_RED,
                __colorB
            );
        }


        void UpdateTile(int __i, Color __colorB=Color::BLACK)
        {
            this->AddLine(
                FixMapX(Game::current_map->X(__i)),
                FixMapY(Game::current_map->Y(__i)),
                Game::current_map->Get(__i)->type == 0 ? "█" : ".",
                Game::current_map->Get(__i)->type == 0 ? Color::WHITE : Color::BRIGHT_BLACK,
                __colorB
            );
        }


        void UpdateCharWithBackground(int __i, Color __colorB)
        {
            for (auto _ent : Game::entities)
                if (_ent->position == __i)
                {
                    UpdateEntity(_ent, __colorB);
                    return;
                }

            UpdateTile(__i, __colorB);
        }
        

        void UpdateWindow()
        {
            for (int _i = 0; _i < Game::current_map->MI; _i++)
                UpdateTile(_i);

            for (auto _ent : Game::entities)
                UpdateEntity(_ent);
        }

        int oldHovX=-1, oldHovY=-1;
        bool OnHover(bool __on, int __x, int __y) override
        {
            __x = GetW() / 2 + GetPlayerDirX(__x - GetW() / 2, __y - GetH() / 2);
            __y = GetH() / 2 + GetPlayerDirY(__x - GetW() / 2, __y - GetH() / 2);
            // old
            if (wmove(this->win, oldHovY, oldHovX) == OK)
            {
                UpdateCharWithBackground(
                    Game::current_map->I(
                        UnFixMapX(oldHovX), 
                        UnFixMapY(oldHovY)
                    ),
                    Color::BLACK
                );
            }
            // new
            if (wmove(this->win, __y, __x) == OK)
            {
                UpdateCharWithBackground(
                    Game::current_map->I(
                        UnFixMapX(__x), 
                        UnFixMapY(__y)
                    ),
                    Color::BLUE
                );
            }

            // update old
            oldHovX = __x;
            oldHovY = __y;
            
            return true;
        }
};
class UIBack : public VisualComponent
{
    public:
        int action;
        UIBack() : VisualComponent(MAPX-1, MAPY-1, 34, 21, VisualComponent::Layer::BACKGROUND, "", Color::BLACK) 
        { std::lock_guard el_lock(lock);

            this->AddLine(0, 0, "┌────────────────────────────────" , Color::WHITE);
            this->AddLine(0, 1, "│\n│\n│\n│\n│\n│\n│\n│\n│\n│\n│\n│\n│\n│\n│\n│\n└", Color::WHITE);
            this->AddLine(33, 0, "┐", Color::BRIGHT_BLACK);
            this->AddLine(33, 1, "│", Color::BRIGHT_BLACK);
            this->AddLine(33, 2, "│", Color::BRIGHT_BLACK);
            this->AddLine(33, 3, "│", Color::BRIGHT_BLACK);
            this->AddLine(33, 4, "│", Color::BRIGHT_BLACK);
            this->AddLine(33, 5, "│", Color::BRIGHT_BLACK);
            this->AddLine(33, 6, "│", Color::BRIGHT_BLACK);
            this->AddLine(33, 7, "│", Color::BRIGHT_BLACK);
            this->AddLine(33, 8, "│", Color::BRIGHT_BLACK);
            this->AddLine(33, 9, "│", Color::BRIGHT_BLACK);
            this->AddLine(33, 10, "│", Color::BRIGHT_BLACK);
            this->AddLine(33, 11, "│", Color::BRIGHT_BLACK);
            this->AddLine(33, 12, "│", Color::BRIGHT_BLACK);
            this->AddLine(33, 13, "│", Color::BRIGHT_BLACK);
            this->AddLine(33, 14, "│", Color::BRIGHT_BLACK);
            this->AddLine(33, 15, "│", Color::BRIGHT_BLACK);
            this->AddLine(33, 16, "│", Color::BRIGHT_BLACK);
            this->AddLine(1, 17, "────────────────────────────────┘" , Color::BRIGHT_BLACK);

            this->AddLine(0, 18, "┌────────────────────────────────" , Color::WHITE);
            this->AddLine(33, 18, "┐", Color::BRIGHT_BLACK);
            this->AddLine(0, 19, "│\n└" , Color::WHITE);
            this->AddLine(33, 19, "│", Color::BRIGHT_BLACK);
            this->AddLine(1, 20, "────────────────────────────────┘" , Color::BRIGHT_BLACK);
            this->AddLine(1, 19, "♥♥♥" , Color::BRIGHT_RED);
            this->AddLine(4, 19, "♥♥♥♥" , Color::RED);
        }
};



void Game::Run()
{
    Engine::Renderer::SetBackground(Color::BLACK);
    {   
        UIButton b1(1, "PLAY", 0);
        UIButton b2(6, "EXIT", 1);
        b1.Activate();
        b2.Activate();
        while (!game_run) {}
    }

    if (entities.size() == 0)
        entities.push_back(new Player(0));

    {
        UIBack b;
        UIMap m;
        m.Activate();
        b.Activate();
        while (game_run) { }
    }

}


void Game::LoadMap(std::string __mapFileName) 
{
    delete Game::current_map;
    Game::current_map = new Map(32, 16, __mapFileName.c_str());
    Game::InitializeMap(Game::current_map);
}


  
void Game::InitializeMap(Map *__map)
{
    Game::ClearMap();
    Game::entities.clear();
    for (int i = 0; i < current_map->MI; i ++)
    {
        // if spawner
        if (current_map->Get(i)->type == 2)
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