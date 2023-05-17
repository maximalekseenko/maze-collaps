#include "player.h"

#include "map.h"
#include "enemy.h"
#include "log.h"

#include <vector>



// (C) Chaos - increases distability
// (W) Wrath - decreases mana, cost decreces ove time no use
// (D) Divinity - uses divinity


// =Effect+= // Single
// (C) Change
// (W) Dig
// (D) Stun

// =Direction= // Double
// (C) Circle
// (W) Diagonal
// (D) Axis

// =Ultimate= // Triple
// (C) Stabilize
// (W) 
// (D) Shield

#define DIR_AMOUNT 1
#define EFF_AMOUNT 2

#define ID_ERR -1
#define ID_C 0
#define ID_W 1
#define ID_D 2

#define EFF_CHANGE ID_C
#define EFF_DIG    ID_W
#define EFF_STUN   ID_D

#define DIR_CIRCLE     ID_C
#define DIR_DIAGONAL   ID_W
#define DIR_AXIS       ID_D

#define CASTCONTINUE true
#define CASTBREAK    false


bool Player::Prep(Player::SpellSource newsource)
{
    if (newsource == Player::SpellSource::NONE) return false;

    sources[0] = sources[1];
    sources[1] = sources[2];
    sources[2] = newsource;

    return true;
}

bool Player::Cast()
{
    int amountC = 0;
    int amountW = 0;
    int amountD = 0;

    for (int i = 0; i < 3; i++)
        switch (sources[i])
        {
            case Player::SpellSource::CHAOS: amountC++; break;
            case Player::SpellSource::WRATH: amountW++; break;
            case Player::SpellSource::DIVINITY: amountD++; break;
            default: return false;
        }

    if (amountC + amountW + amountD != 3) return false;

    // ultimates Cases
    else if (amountC == 3) {}
    else if (amountW == 3) {}
    else if (amountD == 3) {}

    // get effect
    int effect = amountC == EFF_AMOUNT ? ID_C
               : amountW == EFF_AMOUNT ? ID_W
               : amountD == EFF_AMOUNT ? ID_D
               :                         ID_ERR;
    int direction = amountC == DIR_AMOUNT ? ID_C
                  : amountW == DIR_AMOUNT ? ID_W
                  : amountD == DIR_AMOUNT ? ID_D
                  :                         ID_ERR;
    
    Log::Out("Casted " + std::to_string(effect) + " to " + std::to_string(direction));

    auto cast_effect = 
        effect == EFF_DIG ? [](int pos)
        {
            // enemy blocked
            for (auto &enemy : Enemy::enemies)
                if (enemy.position == pos) return CASTBREAK;

            // wall target
            if (Map::Get(pos) == Map::TILE::WALL)
            {
                Map::Get(pos) = Map::TILE::FLOOR;
                return CASTBREAK;
            }

            // nothing
            return CASTCONTINUE;
        }
        // enemy
        : effect == EFF_STUN ? [](int pos)
        {
            // wall block
            if (Map::Get(pos) == Map::TILE::WALL) return CASTBREAK;

            // enemy target
            for (auto &enemy : Enemy::enemies)
                if (enemy.position == pos)
                {
                    enemy.stunTime = 1;
                    return CASTBREAK;
                }
            
            // nothing
            return CASTCONTINUE;
        }
        : [](int pos){ return CASTBREAK; };

    // direction
    if (direction == ID_ERR) return false;
    else if (direction == DIR_CIRCLE)
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
    else if (direction == DIR_AXIS)
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
    else if (direction == DIR_DIAGONAL)
    {
        for ( // LU
            int limit = Map::MX/2, pos = position; 
            limit > 0;
            limit ++
        ) if (cast_effect(Map::Move(&pos, -1, -1)) == CASTBREAK) break;
        for ( // LD
            int limit = Map::MX/2, pos = position; 
            limit > 0; 
            limit ++
        ) if (cast_effect(Map::Move(&pos, -1,  1)) == CASTBREAK) break;
        for ( // RU
            int limit = Map::MX/2, pos = position; 
            limit > 0; 
            limit ++
        ) if (cast_effect(Map::Move(&pos,  1, -1)) == CASTBREAK) break;
        for ( // RD
            int limit = Map::MX/2, pos = position; 
            limit > 0; 
            limit ++
        ) if (cast_effect(Map::Move(&pos,  1,  1)) == CASTBREAK) break;
    }

    sources[0] = SpellSource::NONE;

    return true;
}

bool Player::Move()
{
    return true;
}
