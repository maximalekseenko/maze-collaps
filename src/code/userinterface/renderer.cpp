#include "renderer.h"

#include "../game.h"
#include "../lib/log.h"
#include <ncurses.h>
#include <stdlib.h>

#include "interfaceelement.h"


#define MAP_OFFSET_X 1
#define MAP_OFFSET_Y 1


int mX = -1, mY = -1;

void Renderer::RenderText(int __x, int __y, const char* __text, Color __colorF=Color::WHITE, Color __colorB=Color::BLACK)
{
    attron(COLOR_PAIR(__colorF*8+__colorB));
    mvprintw(__y, __x, __text);
    attroff(COLOR_PAIR(__colorF*8+__colorB));
}

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
    raw();


    clear();
    cbreak();

    InitColors();

    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    Renderer::initialized=true;
}

void Renderer::InitColors()
{
    start_color();
    use_default_colors();

    for (int _iF = 0; _iF < RENDERER_COLOR_AMOUNT; _iF ++)
        for (int _iB = 0; _iB < RENDERER_COLOR_AMOUNT; _iB ++)
            init_pair(_iF * RENDERER_COLOR_AMOUNT + _iB, _iF, _iB);
}



int Renderer::update_hash = 0;

void Renderer::CallForUpdate(int __hash)
{
    Renderer::update_hash = __hash;
}


void Renderer::Run()
{
    while (true)
    {
        Update();
    }
}


void Renderer::Update()
{
    for (int _il = 0; _il < INTERFACE_ELEMENT_LAYERS_AMOUNT; _il ++)
        for (auto &_el : InterfaceElement::layers[_il]) {
            _el->visualComponent.Render(_el->x, _el->y);
        }
    PrintFinish(-1);
}