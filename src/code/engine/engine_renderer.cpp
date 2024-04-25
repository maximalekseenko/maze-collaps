#include "engine.h"


#include <curses.h>
#include <stdlib.h>

#include "curses.h"
#include "visualcomponent.h"
#include "color.h"


// TEMP !
#include "../lib/log.h"



void Engine::Renderer::Init()
{   std::lock_guard curses_locker(Curses::curses_lock);

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
            init_pair(GetColorPairId(_iF, _iB), _iF, _iB);
}


void Engine::Renderer::Run()
{
    while (true)
    {
        // refresh
        {
            std::lock_guard curses_locker(Curses::curses_lock);
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

/// @brief Last applied background color of main window
Color backgroundColor;

void Engine::Renderer::SetBackground(Color __colorB)
{
    backgroundColor = __colorB;
    {   std::lock_guard curses_locker(Curses::curses_lock);
        bkgd(COLOR_PAIR(GetColorPairId(0, __colorB)));
    }
}

Color Engine::Renderer::GetBackground() { return backgroundColor; }