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


    int y = 0;
    mvwaddstr( WGlobal,   y, 0, "┌───────────────────────────────────┐");
    mvwaddstr( WGlobal, ++y, 0, "│ Current map:                      │");
    mvwaddstr( WGlobal, ++y, 0, "│********************..**********   │");
    mvwaddstr( WGlobal, ++y, 0, "│***************..****♙..********   │");
    mvwaddstr( WGlobal, ++y, 0, "│***************..****..*********   │");
    mvwaddstr( WGlobal, ++y, 0, "│***************..*██..█*********   │");
    mvwaddstr( WGlobal, ++y, 0, "│***************..█....██********   │");
    mvwaddstr( WGlobal, ++y, 0, "│****************....♕..█********   │");
    mvwaddstr( WGlobal, ++y, 0, "│**************██.......█********   │");
    mvwaddstr( WGlobal, ++y, 0, "│**************█...♗..♘..█*******   │");
    mvwaddstr( WGlobal, ++y, 0, "│**************█.✪.......█*******   │");
    mvwaddstr( WGlobal, ++y, 0, "│************.......█************   │");
    mvwaddstr( WGlobal, ++y, 0, "│********.......♘..██************   │");
    mvwaddstr( WGlobal, ++y, 0, "│****.♘......██.....*************   │");
    mvwaddstr( WGlobal, ++y, 0, "│.......**█***██..█.*************   │");
    mvwaddstr( WGlobal, ++y, 0, "│█.**█**********██*..************   │");
    mvwaddstr( WGlobal, ++y, 0, "│*******************..***********   │");
    mvwaddstr( WGlobal, ++y, 0, "│*******************...**********   │");
    mvwaddstr( WGlobal, ++y, 0, "│┌───────┐ ┌─────────────────┐      │");
    mvwaddstr( WGlobal, ++y, 0, "│┌───────┐ ┌───────┐ ┌───────┐      │");
    mvwaddstr( WGlobal, ++y, 0, "││       │ │       │ │       │      │");
    mvwaddstr( WGlobal, ++y, 0, "││   .   │<│   .   │<│   .   │<     │");
    mvwaddstr( WGlobal, ++y, 0, "││       │ │       │ │       │      │");
    mvwaddstr( WGlobal, ++y, 0, "│└───────┘ └───────┘ └───────┘      │");
    mvwaddstr( WGlobal, ++y, 0, "│                                   │");
    mvwaddstr( WGlobal, ++y, 0, "│ [wasd]: move                      │");
    mvwaddstr( WGlobal, ++y, 0, "│ [i]: cast '┼', damages ♙ and ♖    │");
    mvwaddstr( WGlobal, ++y, 0, "│ [o]: cast '╳', damages ♘ and ♗    │");
    mvwaddstr( WGlobal, ++y, 0, "│ [p]: cast '◯', damages ♕ and ♔    │");
    mvwaddstr( WGlobal, ++y, 0, "│                                   │");
    mvwaddstr( WGlobal, ++y, 0, "│ Incapaciate all enemies to win    │");
    mvwaddstr( WGlobal, ++y, 0, "│                                   │");
    mvwaddstr( WGlobal, ++y, 0, "│                                   │");
    mvwaddstr( WGlobal, ++y, 0, "└───────────────────────────────────┘");




    // Lable
    mvwaddstr(WGlobal, LABLE_Y, LABLE_X, LABLE_TEXT);

    // slots
    mvwaddstr( WGlobal, WSLOT_Y - 1, WSLOT_X + 1,           TEXT_TYPE);
    mvwaddstr( WGlobal, WSLOT_Y - 1, WSLOT_X + WSLOT_DX + 1, TEXT_DIRECTION);

    // map name
    mvwaddstr( WGlobal, 1, 15, Map::mapName.c_str());

    // // map name
    // mvwaddstr( WGlobal, 1, 15, Map::mapName.c_str());


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

