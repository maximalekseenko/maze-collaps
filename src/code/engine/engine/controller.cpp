#include "../engine.h"

/// @brief Window to read input from.
WINDOW *control_win;

bool is_engine_controller_thread_running = false;
bool is_engine_controller_initialized = false;

void Engine::Controller::Init()
{
    if (is_engine_controller_initialized)
        return;

    {
        std::lock_guard curses_locker(Curses::curses_lock);
        control_win = newwin(0, 0, 0, 0);
        noecho();
        nodelay(control_win, TRUE);
        cbreak();
        keypad(control_win, TRUE);
        mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
        printf("\033[?1003h\n");
    }
}

/// @brief Help function to react on mouse button 1 down
/// @param __x Mouse position on x axis.
/// @param __y Mouse position on y axis.
void HandleMouseButton1(int __x, int __y)
{
    // find on top of what visual component mouse is located.
    {
        std::shared_lock layers_locker(VisualComponent::layers_lock);
        for (int _il = __ENGINE_VISUALCOMPONENT_LAYER_AMOUNT - 1; _il >= 0; _il--)
            for (auto &_el : VisualComponent::layers[_il])
            {
                std::lock_guard el_locker(_el->lock);
                if (_el->Click(__x, __y))
                    return;
            }
    }
}

void HandleMouseMovement(int __x, int __y)
{
    std::shared_lock layers_locker(VisualComponent::layers_lock);

    bool found = false;
    for (int _il = 0; _il < __ENGINE_VISUALCOMPONENT_LAYER_AMOUNT; _il++)
        for (auto &_el : VisualComponent::layers[_il])
        {
            std::lock_guard el_locker(_el->lock);
            _el->Hover(__x, __y);
        }
}

void Engine::Controller::ThreadFunc()
{
    int c;
    MEVENT event;

    is_engine_controller_thread_running = true;
    while (is_engine_controller_thread_running)
    {
        // get input
        {
            std::lock_guard curses_locker(Curses::curses_lock);
            c = wgetch(control_win);
        }

        // check input
        switch (c)
        {

        // mouse
        case KEY_MOUSE:
            if (getmouse(&event) == OK)
            {
                // mouse movement
                HandleMouseMovement(event.x, event.y);

                // mouse button 1
                if (event.bstate & BUTTON1_PRESSED)
                    HandleMouseButton1(event.x, event.y);

                // mouse button 3
                else if (event.bstate & BUTTON3_PRESSED)
                {
                }
            }
            break;
        default:
            break;
        }
    }
}

void Engine::Controller::Deinit()
{
    printf("\033[?1003l\n");
}
void Engine::Controller::StopThreadLoop()
{
    is_engine_controller_thread_running = false;
}