#include "userInterface.h"

#include "map.h"
#include "player.h"
#include "enemy.h"


#define LABLE_TEXT "collaps-maze"
#define LABLE_X 2
#define LABLE_Y 0


#define TEXT_TYPE "type"
#define TEXT_DIRECTION "direction"


#define WGLOBAL_W 37
#define WGLOBAL_H 34
#define WGLOBAL_X 0
#define WGLOBAL_Y 0


#define WMAP_W 32
#define WMAP_H 16
#define WMAP_X 1
#define WMAP_Y 2


#define WSLOT_W 9
#define WSLOT_H 5
#define WSLOT_X 1
#define WSLOT_Y (WMAP_Y + WMAP_H + 1)
#define WSLOT_DX (WSLOT_W + 1)
#define WSLOT_DY 0

#define WSLOT0_X WSLOT_X
#define WSLOT0_Y WSLOT_Y
#define WSLOT1_X (WSLOT0_X + WSLOT_DX)
#define WSLOT1_Y (WSLOT0_Y + WSLOT_DY)
#define WSLOT2_X (WSLOT1_X + WSLOT_DX)
#define WSLOT2_Y (WSLOT1_Y + WSLOT_DY)



WINDOW* UserInterface::WGlobal;
WINDOW* UserInterface::WMap;
WINDOW* UserInterface::WSlot0;
WINDOW* UserInterface::WSlot1;
WINDOW* UserInterface::WSlot2;


void mvwaddline(WINDOW* win, int y, int x, int d, const char* ch, bool horizontal)
{
    if (horizontal)
        for (int i = x; i < x + d; i ++)
            mvwaddstr(win, y, i, ch);
    else
        for (int i = y; i < y + d; i ++)
            mvwaddstr(win, i, y, ch);
        
}


void UserInterface::Init()
{	
    initscr();
    
    // windows
    WGlobal = newwin(WGLOBAL_H, WGLOBAL_W, WGLOBAL_Y, WGLOBAL_X);
    WMap    = newwin(WMAP_H, WMAP_W, WMAP_Y + WGLOBAL_Y, WMAP_X + WGLOBAL_X);
    WSlot0  = newwin(WSLOT_H, WSLOT_W, WSLOT0_Y + WGLOBAL_Y, WSLOT0_X + WGLOBAL_X);
    WSlot1  = newwin(WSLOT_H, WSLOT_W, WSLOT1_Y + WGLOBAL_Y, WSLOT1_X + WGLOBAL_X);
    WSlot2  = newwin(WSLOT_H, WSLOT_W, WSLOT2_Y + WGLOBAL_Y, WSLOT2_X + WGLOBAL_X);
    refresh();
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
    // box(WGlobal, 0, 0);

    // slots
    // //  type
    // mvwaddstr( WGlobal, WSLOT_Y - 1, WSLOT_X,               "┌");
    // mvwaddline(WGlobal, WSLOT_Y - 1, WSLOT_X + 1,           WSLOT_W - 2, "─", true);
    // mvwaddstr( WGlobal, WSLOT_Y - 1, WSLOT_X + WSLOT_W - 1, "┐");
    // mvwaddstr( WGlobal, WSLOT_Y - 1, WSLOT_X + 1,           TEXT_TYPE);
    // //  direction
    // mvwaddstr( WGlobal, WSLOT_Y - 1, WSLOT_X + WSLOT_DX, "┌");
    // mvwaddline(WGlobal, WSLOT_Y - 1, WSLOT_X + WSLOT_DX + 1, WSLOT_W + WSLOT_DX - 2, "─", true);
    // mvwaddstr( WGlobal, WSLOT_Y - 1, WSLOT_X + WSLOT_DX * 2 + WSLOT_W - 1, "┐");
    // mvwaddstr( WGlobal, WSLOT_Y - 1, WSLOT_X + WSLOT_DX + 1, TEXT_DIRECTION);

    // mvwaddstr( WGlobal,  0, 0, "┌───────────────────────────────────┐");
    // mvwaddstr( WGlobal,  1, 0, "│                                   │");
    // mvwaddstr( WGlobal,  2, 0, "│********************..**********   │");
    // mvwaddstr( WGlobal,  3, 0, "│***************..****♙..********   │");
    // mvwaddstr( WGlobal,  4, 0, "│***************..****..*********   │");
    // mvwaddstr( WGlobal,  5, 0, "│***************..*██..█*********   │");
    // mvwaddstr( WGlobal,  6, 0, "│***************..█....██********   │");
    // mvwaddstr( WGlobal,  7, 0, "│****************....♕..█********   │");
    // mvwaddstr( WGlobal,  8, 0, "│**************██.......█********   │");
    // mvwaddstr( WGlobal,  9, 0, "│**************█...♗..♘..█*******   │");
    // mvwaddstr( WGlobal, 10, 0, "│**************█.✪.......█*******   │");
    // mvwaddstr( WGlobal, 11, 0, "│************.......█************   │");
    // mvwaddstr( WGlobal, 12, 0, "│********.......♘..██************   │");
    // mvwaddstr( WGlobal, 13, 0, "│****.♘......██.....*************   │");
    // mvwaddstr( WGlobal, 14, 0, "│.......**█***██..█.*************   │");
    // mvwaddstr( WGlobal, 15, 0, "│█.**█**********██*..************   │");
    // mvwaddstr( WGlobal, 16, 0, "│*******************..***********   │");
    // mvwaddstr( WGlobal, 17, 0, "│*******************...**********   │");
    // mvwaddstr( WGlobal, 18, 0, "│┌───────┐ ┌─────────────────┐      │");
    // mvwaddstr( WGlobal, 19, 0, "│┌───────┐ ┌───────┐ ┌───────┐      │");
    // mvwaddstr( WGlobal, 20, 0, "││       │ │       │ │       │      │");
    // mvwaddstr( WGlobal, 21, 0, "││   .   │ │   .   │ │   .   │      │");
    // mvwaddstr( WGlobal, 22, 0, "││       │ │       │ │       │      │");
    // mvwaddstr( WGlobal, 23, 0, "│└───────┘ └───────┘ └───────┘      │");
    // mvwaddstr( WGlobal, 24, 0, "│                                   │");
    // mvwaddstr( WGlobal, 25, 0, "│                                   │");
    // mvwaddstr( WGlobal, 26, 0, "│                                   │");
    // mvwaddstr( WGlobal, 27, 0, "│                                   │");
    // mvwaddstr( WGlobal, 28, 0, "│                                   │");
    // mvwaddstr( WGlobal, 29, 0, "│                                   │");
    // mvwaddstr( WGlobal, 30, 0, "│                                   │");
    // mvwaddstr( WGlobal, 31, 0, "│                                   │");
    // mvwaddstr( WGlobal, 32, 0, "│                                   │");
    // mvwaddstr( WGlobal, 33, 0, "└───────────────────────────────────┘");

    mvwaddstr( WGlobal,  0, 0, "┌───────────────────────────────────┐");
    mvwaddstr( WGlobal,  1, 0, "│                                   │");
    mvwaddstr( WGlobal,  2, 0, "│********************..**********   │");
    mvwaddstr( WGlobal,  3, 0, "│***************..****♙..********   │");
    mvwaddstr( WGlobal,  4, 0, "│***************..****..*********   │");
    mvwaddstr( WGlobal,  5, 0, "│***************..*██..█*********   │");
    mvwaddstr( WGlobal,  6, 0, "│***************..█....██********   │");
    mvwaddstr( WGlobal,  7, 0, "│****************....♕..█********   │");
    mvwaddstr( WGlobal,  8, 0, "│**************██.......█********   │");
    mvwaddstr( WGlobal,  9, 0, "│**************█...♗..♘..█*******   │");
    mvwaddstr( WGlobal, 10, 0, "│**************█.✪.......█*******   │");
    mvwaddstr( WGlobal, 11, 0, "│************.......█************   │");
    mvwaddstr( WGlobal, 12, 0, "│********.......♘..██************   │");
    mvwaddstr( WGlobal, 13, 0, "│****.♘......██.....*************   │");
    mvwaddstr( WGlobal, 14, 0, "│.......**█***██..█.*************   │");
    mvwaddstr( WGlobal, 15, 0, "│█.**█**********██*..************   │");
    mvwaddstr( WGlobal, 16, 0, "│*******************..***********   │");
    mvwaddstr( WGlobal, 17, 0, "│*******************...**********   │");
    mvwaddstr( WGlobal, 18, 0, "│┌───────┐ ┌─────────────────┐      │");
    mvwaddstr( WGlobal, 19, 0, "│┌───────┐ ┌───────┐ ┌───────┐      │");
    mvwaddstr( WGlobal, 20, 0, "││││ . │││<│   .   │<│   .   │<     │");
    mvwaddstr( WGlobal, 21, 0, "│└───────┘ └───────┘ └───────┘      │");
    mvwaddstr( WGlobal, 22, 0, "│                                   │");
    mvwaddstr( WGlobal, 23, 0, "│                                   │");
    mvwaddstr( WGlobal, 24, 0, "│                                   │");
    mvwaddstr( WGlobal, 25, 0, "│                                   │");
    mvwaddstr( WGlobal, 26, 0, "│                                   │");
    mvwaddstr( WGlobal, 27, 0, "│                                   │");
    mvwaddstr( WGlobal, 28, 0, "│                                   │");
    mvwaddstr( WGlobal, 29, 0, "│                                   │");
    mvwaddstr( WGlobal, 30, 0, "│                                   │");
    mvwaddstr( WGlobal, 31, 0, "│                                   │");
    mvwaddstr( WGlobal, 32, 0, "│                                   │");
    mvwaddstr( WGlobal, 33, 0, "└───────────────────────────────────┘");


    // Lable
    mvwaddstr(WGlobal, LABLE_Y, LABLE_X, LABLE_TEXT);


    wrefresh(WGlobal);
}


int MapFixedX(int i) { return (Map::MX * 3/2 + Map::X(i) - Map::X(Player::player.position)) % Map::MX + WMAP_W / 2 - Map::MX / 2; }
int MapFixedY(int i) { return (Map::MY * 3/2 + Map::Y(i) - Map::Y(Player::player.position)) % Map::MY + WMAP_H / 2 - Map::MY / 2; }


void UserInterface::DrawMap()
{
    bool SEE = false;

    // tiles
    for (int i = 0; i < Map::MI; i ++)
        if (SEE || Map::IsLineOfSight(Player::player.position, i))
            if (Map::Get(i) == Map::TILE::WALL)
                mvwprintw(WMap, MapFixedY(i), MapFixedX(i), "█");
            else  mvwprintw(WMap, MapFixedY(i), MapFixedX(i), ".");
        else  mvwprintw(WMap, MapFixedY(i), MapFixedX(i), "*");
        
    
    // enemies
    for (auto enemy : Enemy::enemies)
        if (SEE || Map::IsLineOfSight(Player::player.position, enemy.position))
            mvwprintw(WMap, MapFixedY(enemy.position), MapFixedX(enemy.position), enemy.GetVisual());

    // player
    mvwprintw(WMap, MapFixedY(Player::player.position), MapFixedX(Player::player.position), Player::player.visual);


    wrefresh(WMap);
}

const char* GetPlayerElement(int i)
{
    switch (Player::player.slots[i])
    {
    case Player::Element::A: return "┼";
    case Player::Element::B: return "╳";
    case Player::Element::C: return "◯";
    default: return ".";
    }
}


void UserInterface::DrawSlots()
{
    box(WSlot0, 0, 0);
    box(WSlot1, 0, 0);
    box(WSlot2, 0, 0);
    mvwprintw(WSlot0, 2, 4, GetPlayerElement(0));
    mvwprintw(WSlot1, 2, 4, GetPlayerElement(1));
    mvwprintw(WSlot2, 2, 4, GetPlayerElement(2));

    wrefresh(WSlot0);
    wrefresh(WSlot1);
    wrefresh(WSlot2);
}

int UserInterface::Input()
{
    move(WGLOBAL_H + WGLOBAL_Y, 0);
    return getch();
}

