#include "renderer.h"

#include "../game.h"
#include "../lib/log.h"
#include <ncurses.h>
#include <stdlib.h>

#include "interfaceelement.h"
#include "userinterface.h"



// TEMP ?
int colornum(int fg, int bg)
{
    int B, bbb, ffff;

    B = 1 << 7;
    bbb = (7 & bg) << 4;
    ffff = 7 & fg;

    return (B | bbb | ffff);
}

void Renderer::RenderText(int __x, int __y, const char* __text, Color __colorF, Color __colorB)
{
    std::lock_guard ncurses_locker(UserInterface::ncurses_lock);

    attron(COLOR_PAIR(colornum(__colorF, __colorB)));
    if (__colorF / RENDERER_COLOR_AMOUNT == 1) attron(A_BOLD);

    mvprintw(__y, __x, __text);

    attroff(COLOR_PAIR(colornum(__colorF, __colorB)));
    if (__colorF / RENDERER_COLOR_AMOUNT == 1) attroff(A_BOLD);
}

int MapFixedX(int __i, Map* __map, int __center) { return (__map->MX * 3/2 + __map->X(__i) - __map->X(__center)) % __map->MX; }
int MapFixedY(int __i, Map* __map, int __center) { return (__map->MY * 3/2 + __map->Y(__i) - __map->Y(__center)) % __map->MY; }


bool Renderer::initialized=false;
void Renderer::Init()
{
    std::lock_guard ncurses_locker(UserInterface::ncurses_lock);
    initscr();

    curs_set(0);

    clear();

    InitColors();

    Renderer::initialized=true;
}

void Renderer::InitColors()
{
    std::lock_guard ncurses_locker(UserInterface::ncurses_lock);
    
    use_default_colors();
    start_color();

    for (int _iF = 0; _iF < RENDERER_COLOR_AMOUNT; _iF ++)
        for (int _iB = 0; _iB < RENDERER_COLOR_AMOUNT; _iB ++)
            init_pair(colornum(_iF, _iB), _iF, _iB);
}



int Renderer::update_hash = 0;

void Renderer::CallForUpdate(int __hash)
{
    Renderer::update_hash = __hash;
}


bool Renderer::is_running = false;
void Renderer::Run()
{
    is_running = true;
    while (is_running)
    {
        Update();
    }
}


void Renderer::Update()
{

    // Log::Out("RND: RENDERS");
    for (int _il = 0; _il < INTERFACE_ELEMENT_LAYERS_AMOUNT; _il ++)
        for (auto &_el : InterfaceElement::layers[_il])
        {
            std::lock_guard el_locker(_el->lock);
            _el->visualComponent.Render(_el->x, _el->y);
        }
        
    std::lock_guard ncurses_locker(UserInterface::ncurses_lock);
    refresh();
}