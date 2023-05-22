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


bool Player::Turn()
{
    // get action
    std::string task;

    int button = UserInterface::Input();
    Log::Out(std::to_string(button));

    // do action
    if (button == BUTTONEXIT) throw std::runtime_error("GAME QUIT");

    if (button == BUTTONMU) return Move( 0, -1);
    if (button == BUTTONMD) return Move( 0,  1);
    if (button == BUTTONML) return Move(-1,  0);
    if (button == BUTTONMR) return Move( 1,  0);

    if (button == BUTTONCA) return Cast(Element::A);
    if (button == BUTTONCB) return Cast(Element::B);
    if (button == BUTTONCC) return Cast(Element::C);

    return TURNCONTINUE;
}


bool Player::Cast(Player::Element newsource)
{
    slots[0] = slots[1];
    slots[1] = slots[2];
    slots[2] = newsource;


    // no cast
    if (slots[0] == Element::NONE || slots[1] == Element::NONE || slots[2] == Element::NONE) return TURNBREAK;
    Log::Out("A");

    // no direction
    if (slots[1] != slots[2]) return TURNBREAK;
    Log::Out("B");


    // next map
    if (slots[0] != slots[1] && slots[1] != slots[2] && slots[2] != slots[0])
        return TURNBREAK;
    Log::Out("C");

    // effect
    auto cast_effect = [this](int pos)
    {
        // wall block
        if (Map::Get(pos) == Map::TILE::WALL) return CASTBREAK;

        // enemy target
        for (auto &enemy : Enemy::enemies)
            if (enemy.position == pos)
            {
                enemy.Kill(this->slots[0]);
                Log::Out("Hit Enemy at " + std::to_string(enemy.name));
                return CASTBREAK;
            }
        
        // nothing
        return CASTCONTINUE;
    };

    // if (direction == ID_ERR) return false;
    Log::Out(std::to_string(slots[2] == CAST_DIR_R) + " " + std::to_string(slots[2] == CAST_DIR_A) + " " + std::to_string(slots[2] == CAST_DIR_D));
    if (slots[2] == CAST_DIR_R)
    {
        int pos;
        cast_effect(Map::Move(position, -1,  0)); // L
        cast_effect(Map::Move(position,  1,  0)); // R
        cast_effect(Map::Move(position,  0, -1)); // U
        cast_effect(Map::Move(position,  0,  1)); // D
        cast_effect(Map::Move(position, -1, -1)); // LU
        cast_effect(Map::Move(position, -1,  1)); // LD
        cast_effect(Map::Move(position,  1, -1)); // RU
        cast_effect(Map::Move(position,  1,  1)); // RD
    }
    else if (slots[2] == CAST_DIR_A)
    {
        for ( // L
            int limit = Map::MX/2, pos = position; 
            limit > 0; 
            limit ++
        ) if (cast_effect(Map::Move(&pos, -1,  0)) == CASTBREAK) break;
        for ( // R
            int limit = Map::MX/2, pos = position; 
            limit > 0; 
            limit ++
        ) if (cast_effect(Map::Move(&pos,  1,  0)) == CASTBREAK) break;
        for ( // U
            int limit = Map::MX/2, pos = position; 
            limit > 0; 
            limit ++
        ) if (cast_effect(Map::Move(&pos,  0, -1)) == CASTBREAK) break;
        for ( // D
            int limit = Map::MX/2, pos = position; 
            limit > 0; 
            limit ++
        ) if (cast_effect(Map::Move(&pos,  0,  1)) == CASTBREAK) break;
    }
    else if (slots[2] == CAST_DIR_D)
    // {
    //     for ( // LU
    //         int limit = Map::MX/2, pos = position; 
    //         limit > 0;
    //         limit ++
    //     ) if (cast_effect(Map::Move(&pos, -1, -1)) == CASTBREAK) break;
    //     for ( // LD
    //         int limit = Map::MX/2, pos = position; 
    //         limit > 0; 
    //         limit ++
    //     ) if (cast_effect(Map::Move(&pos, -1,  1)) == CASTBREAK) break;
    //     for ( // RU
    //         int limit = Map::MX/2, pos = position; 
    //         limit > 0; 
    //         limit ++
    //     ) if (cast_effect(Map::Move(&pos,  1, -1)) == CASTBREAK) break;
    //     for ( // RD
    //         int limit = Map::MX/2, pos = position; 
    //         limit > 0; 
    //         limit ++
    //     ) if (cast_effect(Map::Move(&pos,  1,  1)) == CASTBREAK) break;
    // }

    slots[0] = Element::NONE;
    slots[1] = Element::NONE;
    slots[2] = Element::NONE;
    return TURNBREAK;
}

bool Player::Move(int dx, int dy)
{
    if (!Map::IsNotObstacle(Map::Move(position, dx, dy))) return TURNCONTINUE;

    Map::Move(&position, dx, dy);
    Cast(Element::NONE);
    return TURNBREAK;
}
