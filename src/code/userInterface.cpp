#include "userInterface.h"

#include "map.h"
#include "player.h"
#include "enemy.h"


#define LABLE_TEXT "collaps-maze"
#define LABLE_X 2
#define LABLE_Y 0


#define TEXT_TYPE "type"
#define TEXT_DIRECTION "direction"


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
    // Lable
    mvwaddstr(WGlobal, LABLE_Y, LABLE_X, LABLE_TEXT);

    // slots
    mvwaddstr(WGlobal, WSLOT_Y - 1 - WGLOBAL_Y, WSLOT_X - WGLOBAL_X, TEXT_TYPE);


    wrefresh(WGlobal);
}


int FixedX(int i) { return (Map::MX * 3/2 + Map::X(i) - Map::X(Player::player.position)) % Map::MX; }
int FixedY(int i) { return (Map::MY * 3/2 + Map::Y(i) - Map::Y(Player::player.position)) % Map::MY; }


void UserInterface::DrawMap()
{
    bool SEE = false;

    // tiles
    for (int i = 0; i < Map::MI; i ++)
        if (SEE || Map::IsLineOfSight(Player::player.position, i))
            if (Map::Get(i) == Map::TILE::WALL)
                mvwprintw(WMap, FixedY(i), FixedX(i), "█");
            else  mvwprintw(WMap, FixedY(i), FixedX(i), ".");
        else  mvwprintw(WMap, FixedY(i), FixedX(i), "*");
        
    
    // enemies
    for (auto enemy : Enemy::enemies)
        if (SEE || Map::IsLineOfSight(Player::player.position, enemy.position))
            mvwprintw(WMap, FixedY(enemy.position), FixedX(enemy.position), enemy.GetVisual());

    // player
    mvwprintw(WMap, FixedY(Player::player.position), FixedX(Player::player.position), Player::player.visual);


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
    mvwprintw(WSlot0, 2, 5, GetPlayerElement(0));
    mvwprintw(WSlot1, 2, 5, GetPlayerElement(1));
    mvwprintw(WSlot2, 2, 5, GetPlayerElement(2));

    wrefresh(WSlot0);
    wrefresh(WSlot1);
    wrefresh(WSlot2);
}

int UserInterface::Input()
{
    move(WGLOBAL_H + WGLOBAL_Y, 0);
    return getch();
}

