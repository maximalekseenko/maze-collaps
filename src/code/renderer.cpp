#include "renderer.h"

#include "game.h"
#include "lib/log.h"
#include <ncurses.h>


Renderer Renderer::renderer = Renderer();


void PrintFinish(int __y=0) 
{ 
    move(__y,0); 
    refresh();
}
int MapFixedX(int __i, Map* __map, int __center) { return (__map->MX * 3/2 + __map->X(__i) - __map->X(__center)) % __map->MX; }
int MapFixedY(int __i, Map* __map, int __center) { return (__map->MY * 3/2 + __map->Y(__i) - __map->Y(__center)) % __map->MY; }


bool Renderer::initialized=false;
void Renderer::Init()
{
    initscr();
    refresh();
    Renderer::initialized=true;
}

void Renderer::Render_Game(
    int __center,
    bool __safe
)
{
    if (!initialized) return;

    Render_Map(
        Game::game.current_map,
        0, 0,
        __center, __safe
    );

    for (auto _ent : Game::game.entities)
        Render_Entity(
            Game::game.current_map, 
            _ent,
            0, 0,
            __center, __safe
        );

    PrintFinish(Game::game.current_map->MY);
}

void Renderer::Render_Map(
    Map *__map, 
    int __offset_x,
    int __offset_y,
    int __center,
    bool __safe
)
{
    if (!initialized) return;

    for (int _i = 0; _i < __map->MI; _i ++)
        mvprintw(
            MapFixedY(_i, __map, __center) + __offset_y,
            MapFixedX(_i, __map, __center) + __offset_x,
            (__map->Get(_i) == Map::TILE::WALL ? "█" : ".")
        );
    if (__safe) PrintFinish(__map->MY);
}
void Renderer::Render_Entity(
    Map *__map,
    Entity *__ent,
    int __offset_x,
    int __offset_y,
    int __center,
    bool __safe
)
{
    if (!initialized) return;

    mvprintw(
        MapFixedY(__ent->position, __map, __center) + __offset_y,
        MapFixedX(__ent->position, __map, __center) + __offset_x, 
        __ent->GetVisual()
    );
    if (__safe) PrintFinish(__map->MY);
}
