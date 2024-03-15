#include "player.h"

#include "map.h"
#include "enemy.h"
#include "log.h"
#include "userInterface.h"

#include <vector>


#define BUTTONEXIT 27

#define BUTTONML 97
#define BUTTONMR 100
#define BUTTONMD 115
#define BUTTONMU 119

#define BUTTONCA 105
#define BUTTONCB 111
#define BUTTONCC 112


#define CAST_DIR_A Player::Element::A
#define CAST_DIR_D Player::Element::B
#define CAST_DIR_R Player::Element::C


#define CASTCONTINUE true
#define CASTBREAK    false




Player Player::player;


bool Player::Turn(Map* __map)
{
    // get action
    std::string task;

    int button = UserInterface::Input();
    Log::Out("--LOG: Player button press: " + std::to_string(button));

    // do action
    if (button == BUTTONEXIT) throw std::runtime_error("GAME QUIT");

    if (button == BUTTONMU) return Move( 0, -1, __map);
    if (button == BUTTONMD) return Move( 0,  1, __map);
    if (button == BUTTONML) return Move(-1,  0, __map);
    if (button == BUTTONMR) return Move( 1,  0, __map);

    if (button == BUTTONCA) return Cast(Element::A, __map);
    if (button == BUTTONCB) return Cast(Element::B, __map);
    if (button == BUTTONCC) return Cast(Element::C, __map);

    Log::Out("--LOG: Player button not found: " + std::to_string(button));
    return TURNCONTINUE;
}


bool Player::Cast(Player::Element nextelement, Map* __map)
{
    // add element
    slots[0] = slots[1];
    slots[1] = slots[2];
    slots[2] = nextelement;

    Log::Out("--LOG: Player element added: " + std::to_string(nextelement));


    // if not enough elements
    if (slots[0] == Element::NONE || slots[1] == Element::NONE || slots[2] == Element::NONE) return TURNBREAK;

    // if different elements in direction
    if (slots[1] != slots[2]) return TURNBREAK;
    
    // effect
    auto cast_effect = [this](int pos)
    {
        // wall block
        if (__map->Get(pos) == Map::TILE::WALL) return CASTBREAK;

        // enemy target
        for (auto &enemy : Enemy::enemies)
        {
            if (enemy.position == pos)
            {
                enemy.Kill(this->slots[0]);
                return CASTBREAK;
            }
        }
        
        // nothing
        return CASTCONTINUE;
    };

    if (slots[2] == CAST_DIR_R)
    {
        // int pos;
        cast_effect(__map->Move(position, -1,  0)); // L
        cast_effect(__map->Move(position,  1,  0)); // R
        cast_effect(__map->Move(position,  0, -1)); // U
        cast_effect(__map->Move(position,  0,  1)); // D
        cast_effect(__map->Move(position, -1, -1)); // LU
        cast_effect(__map->Move(position, -1,  1)); // LD
        cast_effect(__map->Move(position,  1, -1)); // RU
        cast_effect(__map->Move(position,  1,  1)); // RD
    }
    else if (slots[2] == CAST_DIR_A)
    {
        for ( // L
            int limit = 0, pos = position; 
            limit < __map->MX/2; 
            limit ++
        ) if (cast_effect(__map->Move(&pos, -1,  0)) == CASTBREAK) break;
        for ( // R
            int limit = 0, pos = position; 
            limit < __map->MX/2; 
            limit ++
        ) if (cast_effect(__map->Move(&pos,  1,  0)) == CASTBREAK) break;
        for ( // U
            int limit = 0, pos = position; 
            limit < __map->MY/2; 
            limit ++
        ) if (cast_effect(__map->Move(&pos,  0, -1)) == CASTBREAK) break;
        for ( // D
            int limit = 0, pos = position; 
            limit < __map->MY/2; 
            limit ++
        ) if (cast_effect(__map->Move(&pos,  0,  1)) == CASTBREAK) break;
    }
    else if (slots[2] == CAST_DIR_D)
    {
        int _max_limit = std::min(__map->MX/2, __map->MY/2);
        for ( // LU
            int limit = 0, pos = position; 
            limit < _max_limit;
            limit ++
        ) if (cast_effect(__map->Move(&pos, -1, -1)) == CASTBREAK) break;
        for ( // LD
            int limit = 0, pos = position; 
            limit < _max_limit;
            limit ++
        ) if (cast_effect(__map->Move(&pos, -1,  1)) == CASTBREAK) break;
        for ( // RU
            int limit = 0, pos = position; 
            limit < _max_limit;
            limit ++
        ) if (cast_effect(__map->Move(&pos,  1, -1)) == CASTBREAK) break;
        for ( // RD
            int limit = 0, pos = position; 
            limit < _max_limit;
            limit ++
        ) if (cast_effect(__map->Move(&pos,  1,  1)) == CASTBREAK) break;
    }

    slots[0] = Element::NONE;
    slots[1] = Element::NONE;
    slots[2] = Element::NONE;
    return TURNBREAK;
}

bool Player::Move(int dx, int dy, Map* __map)
{
    if (!__map->IsNotObstacle(__map->Move(position, dx, dy))) return TURNCONTINUE;

    __map->Move(&position, dx, dy);
    Log::Out("--LOG: Player moved from ["
         + std::to_string(__map->X(position) - dx) + " " + std::to_string(__map->Y(position) - dy)
         + "] to ["
         + std::to_string(__map->X(position)) + " " + std::to_string(__map->Y(position))
         + "]");

    Cast(Element::NONE, __map);

    return TURNBREAK;
}
