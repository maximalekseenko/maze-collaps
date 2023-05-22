#include "userInterface.h"


#define LABLE_TEXT "collaps-maze"
#define LABLE_X 2
#define LABLE_Y 0


#define WGLOBAL_W 34
#define WGLOBAL_H 34
#define WGLOBAL_X 0
#define WGLOBAL_Y 0


#define WMAP_W 32
#define WMAP_H 16
#define WMAP_X (WGLOBAL_X + 1)
#define WMAP_Y (WGLOBAL_Y + 2)


#define WSLOT_W 10
#define WSLOT_H 5
#define WSLOT_X (WGLOBAL_X + 1)
#define WSLOT_Y (WMAP_Y + WMAP_H + 1)
#define WSLOT_DX (WSLOT_W + 1)
#define WSLOT_DY 0


WINDOW* UserInterface::WGlobal;
WINDOW* UserInterface::WMap;
WINDOW* UserInterface::WSlot0;
WINDOW* UserInterface::WSlot1;
WINDOW* UserInterface::WSlot2;


void UserInterface::Init()
{	
    initscr();
    
    // windows
    WGlobal = newwin(WGLOBAL_H, WGLOBAL_W, WGLOBAL_Y, WGLOBAL_X);
    WMap    = newwin(WMAP_H, WMAP_W, WMAP_Y, WMAP_X);
    WSlot0  = newwin(WSLOT_H, WSLOT_W, WSLOT_Y + WSLOT_DY * 0, WSLOT_X + WSLOT_DX * 0);
    WSlot1  = newwin(WSLOT_H, WSLOT_W, WSLOT_Y + WSLOT_DY * 1, WSLOT_X + WSLOT_DX * 1);
    WSlot2  = newwin(WSLOT_H, WSLOT_W, WSLOT_Y + WSLOT_DY * 2, WSLOT_X + WSLOT_DX * 2);
    refresh();


    UpdateAll();

    getch();
}


void UserInterface::Exit()
{
    endwin();
}    


void UserInterface::Update()
{
    DrawMap();
    DrawSlots();
}


void UserInterface::UpdateAll()
{
    DrawBackground();
    Update();
}


void UserInterface::DrawBackground()
{
    box(WGlobal, 0, 0);
    mvwaddstr(WGlobal, LABLE_Y, LABLE_X, LABLE_TEXT);
    wrefresh(WGlobal);
}


void UserInterface::DrawMap()
{

}


void UserInterface::DrawSlots()
{

}

char UserInterface::Input()
{
    return getch();
}

