#include "controls.h"

#include <ncurses.h>

#include "visualcomponent.h"
#include "userinterface.h"

#include "../lib/log.h"

WINDOW* control_win;
void Controls::Init()
{
    std::lock_guard ncurses_locker(UserInterface::ncurses_lock);

    control_win = newwin(0, 0, 0, 0);

    noecho();
    nodelay(control_win, TRUE);
    cbreak();
    keypad(control_win, TRUE);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    // printf("\033[?1003l\n");
    printf("\033[?1003h\n");
}

bool PointWithinBorders(int __px, int __py, int __bx, int __by, int __bw, int __bh)
{
    return __bx <= __px 
        && __bx + __bw > __px
        && __by <= __py 
        && __by + __bh > __py;
}

void HandleMouseButton1(int __x, int __y)
{
    std::lock_guard layers_locker(VisualComponent::layers_lock);
    for (int _il = 0; _il < VISUALCOMPONENT_LAYER_AMOUNT; _il ++)
        for (auto &_el : VisualComponent::layers[_il])
        {
            std::lock_guard<std::recursive_mutex> locker(_el->lock);
            if (PointWithinBorders(__x, __y, _el->GetX(), _el->GetY(), _el->GetW(), _el->GetH()))
            {
                _el->Click(true, __x, __y);
                return;
            }
        }
}


void HandleMouseMovement(int __x, int __y)
{
    std::lock_guard layers_locker(VisualComponent::layers_lock);

    bool found=false;
    for (int _il = 0; _il < VISUALCOMPONENT_LAYER_AMOUNT; _il ++)
        for (auto &_el : VisualComponent::layers[_il])
        {
            std::lock_guard el_locker(_el->lock);
            if (!found && PointWithinBorders(__x, __y, _el->GetX(), _el->GetY(), _el->GetW(), _el->GetH()))
            {
                _el->Hover(true, __x, __y);
            }
            else
                _el->Hover(false, __x, __y);
        }
}


void Controls::Run() 
{
    int c;
    MEVENT event;

    while (true) 
    {
        // get input
        {
            std::lock_guard ncurses_locker(UserInterface::ncurses_lock);
            c = wgetch(control_win);
        }

        // check input
        switch(c)
        {

            // mouse
            case KEY_MOUSE:
                if(getmouse(&event) == OK)
                {
                    // mouse movement
                    HandleMouseMovement(event.x, event.y);

                    // mouse button 1
                    if(event.bstate & BUTTON1_PRESSED)
                        HandleMouseButton1(event.x, event.y);

                    // mouse button 3
                    else if(event.bstate & BUTTON3_PRESSED)
                        Log::Out("CON: b3");
                }
                break;
            default:
                break;
        }
    }
}