#include "controls.h"

#include <ncurses.h>

#include "interfaceelement.h"


void Controls::Init()
{
    keypad(stdscr, TRUE);
    printf("\033[?1003h\n");
    // printf("\033[?1003l\n");
    noecho();
}

void Controls::Run() 
{
    
}