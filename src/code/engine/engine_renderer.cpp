#include "engine.h"

#include <stdlib.h>

#include "curses.h"
#include "visualcomponent.h"
#include "color.h"

// TEMP !
#include "../lib/log.h"

bool is_engine_renderer_thread_running = false;
bool is_engine_renderer_initialized = false;

void Engine::Renderer::Init()
{
    // argument checks
    // -if already initialized
    if (is_engine_renderer_initialized)
        Log::Out("Initializing Engine::Renderer, when it is already initialized.", true);

    // do the initializations
    {
        std::lock_guard curses_locker(Curses::curses_lock);

        // --- init screen ---
        // init core screen
        initscr();
        // remove cursor form screen
        curs_set(0);
        // clear screen
        clear();

        // --- init colors ---
        use_default_colors();
        start_color();
        for (int _iF = 0; _iF < 8; _iF++)
            for (int _iB = 0; _iB < 8; _iB++)
                init_pair(GetColorPairId(_iF, _iB), _iF, _iB);
    }
}

void Engine::Renderer::Deinit()
{
    // argument checks
    // -if not initialized
    if (!is_engine_renderer_initialized)
        Log::Out("Deinitializing Engine::Renderer before initialization.", true);

    /// @note rn is done with endwin() at engine
    // // do the deinitializations
    // {
    //     std::lock_guard curses_locker(Curses::curses_lock);

    //     curs_set(1);

    //     // --- init colors ---
    //     use_default_colors();
    //     start_color();
    //     for (int _iF = 0; _iF < 8; _iF++)
    //         for (int _iB = 0; _iB < 8; _iB++)
    //             init_pair(GetColorPairId(_iF, _iB), _iF, _iB);
    // }
}

void Engine::Renderer::ThreadFunc()
{
    // argument checks
    // -if already running
    if (is_engine_renderer_thread_running)
        Log::Out("Attempt at running multiple Engine::Renderer threads", true);

    // start the loop
    is_engine_renderer_thread_running = true;
    while (is_engine_renderer_thread_running)
    {
        // refresh
        {
            std::lock_guard curses_locker(Curses::curses_lock);
            refresh();
        }

        // render layers
        {
            std::shared_lock layers_locker(VisualComponent::layers_lock);

            // check that render is required
            if (VisualComponent::lastUpdatedLayer == VisualComponent::Layer::NONE)
                continue;

            for (int _il = VisualComponent::lastUpdatedLayer; _il < __ENGINE_VISUALCOMPONENT_LAYER_AMOUNT; _il++)
                for (auto &_el : VisualComponent::layers[_il])
                {
                    std::lock_guard el_locker(_el->lock);
                    if (VisualComponent::lastUpdateMinX > _el->GetMaxX() || VisualComponent::lastUpdateMaxX < _el->GetX())
                        continue;
                    if (VisualComponent::lastUpdateMinY > _el->GetMaxY() || VisualComponent::lastUpdateMaxY < _el->GetY())
                        continue;
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
void Engine::Renderer::StopThreadLoop()
{
    // argument checks
    // -if loop is already broken
    if (!is_engine_renderer_thread_running)
        Log::Out("Attempt at stopping Engine::Renderer thread, when there is none", true);

    // set loop break flag
    is_engine_renderer_thread_running = false;
}

/// @brief Last applied background color of main window
Color backgroundColor;

void Engine::Renderer::SetBackground(Color __colorB)
{
    backgroundColor = __colorB;
    {
        std::lock_guard curses_locker(Curses::curses_lock);
        bkgd(COLOR_PAIR(GetColorPairId(0, __colorB)));
    }
}

Color Engine::Renderer::GetBackground() { return backgroundColor; }