#include "engine.h"


#include "../game.h"
#include "../lib/log.h"
#include <ncurses.h>
#include <stdlib.h>

#include "visualcomponent.h"


// TEMP !
#include "../lib/log.h"



// TEMP ?
int colornum(int fg, int bg)
{
    int B, bbb, ffff;

    B = 1 << 7;
    bbb = (7 & bg) << 4;
    ffff = 7 & fg;

    return (B | bbb | ffff);
}



void Engine::Renderer::RenderText(WINDOW* __win, int __x, int __y, const char* __text, Color __colorF, Color __colorB)
{   std::lock_guard ncurses_locker(Engine::ncurses_lock);

    // set color
    wattron(__win, COLOR_PAIR(colornum(__colorF, __colorB)));
    if (__colorF / 8 == 1) wattron(__win, A_BOLD);
    else                  wattroff(__win, A_BOLD);

    // print
    mvwprintw(__win, __y, __x, __text);
}



void Engine::Renderer::Init()
{   std::lock_guard ncurses_locker(Engine::ncurses_lock);

    // --- init screen ---
    // init core screen
    initscr();
    // no cursor
    curs_set(0);
    // clear screen
    clear();

    // --- init colors ---
    use_default_colors();
    start_color();
    for (int _iF = 0; _iF < 8; _iF ++)
        for (int _iB = 0; _iB < 8; _iB ++)
            init_pair(colornum(_iF, _iB), _iF, _iB);
}


void Engine::Renderer::Run()
{
    while (true)
    {
        // refresh
        {
            std::lock_guard ncurses_locker(Engine::ncurses_lock);
            refresh();
        }

        // render layers
        {   std::shared_lock layers_locker(VisualComponent::layers_lock);

            // check that render is required
            if (VisualComponent::lastUpdatedLayer == VisualComponent::Layer::NONE) continue;

            for (int _il = VisualComponent::lastUpdatedLayer; _il < __ENGINE_VISUALCOMPONENT_LAYER_AMOUNT; _il ++)
                for (auto &_el : VisualComponent::layers[_il])
                {   std::lock_guard el_locker(_el->lock);
                    if (VisualComponent::lastUpdateMinX > _el->GetMaxX() || VisualComponent::lastUpdateMaxX < _el->GetX()) continue;
                    if (VisualComponent::lastUpdateMinY > _el->GetMaxY() || VisualComponent::lastUpdateMaxY < _el->GetY()) continue;
                    _el->Render();
                }

            // everything is rendered -> reset
            VisualComponent::lastUpdatedLayer = VisualComponent::Layer::NONE;
            VisualComponent::lastUpdateMinX = INT_MAX;
            VisualComponent::lastUpdateMinY = INT_MAX;
            VisualComponent::lastUpdateMaxX = 0;
            VisualComponent::lastUpdateMaxY = 0;
        }
    }
}