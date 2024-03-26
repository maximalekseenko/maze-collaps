#include "controls.h"

#include <ncurses.h>

#include "interfaceelement.h"
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
    std::lock_guard layers_locker(InterfaceElement::layers_lock);
    for (int _il = 0; _il < INTERFACE_ELEMENT_LAYERS_AMOUNT; _il ++)
        for (auto &_el : InterfaceElement::layers[_il])
        {
            std::lock_guard<std::recursive_mutex> locker(_el->lock);
            if (PointWithinBorders(__x, __y, _el->x, _el->y, _el->w, _el->h))
            {
                _el->Click(true);
                return;
            }
        }
}


void HandleMouseMovement(int __x, int __y)
{
    std::lock_guard layers_locker(InterfaceElement::layers_lock);

    bool found=false;
    for (int _il = 0; _il < INTERFACE_ELEMENT_LAYERS_AMOUNT; _il ++)
        for (auto &_el : InterfaceElement::layers[_il])
        {
            std::lock_guard<std::recursive_mutex> locker(_el->lock);
            if (!found && PointWithinBorders(__x, __y, _el->x, _el->y, _el->w, _el->h))
            {
                _el->Hover(true);
            }
            else
                _el->Hover(false);
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