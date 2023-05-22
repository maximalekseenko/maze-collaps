#include "player.h"

#include "map.h"
#include "enemy.h"
#include "log.h"

#include <vector>


#define CASTCONTINUE true
#define CASTBREAK    false

Player Player::player;

// bool Player::Turn()
// {
//     // get action
//     std::string task;
//     // getline(std::cin, task);
//     getchar();

//     // do action
//     if (task == "quit") throw std::runtime_error("GAME QUIT");
//     if (task == "move")


//     while (true)
//     {  
//         // get task and args 
//         getline(std::cin, task);

//         // check command
//         /// system
//         if (task == "quit") break;

//         /// movement
//         else if (task == "mu" || task == "move up"   ) 
//         {
//             if (!Map::IsNotObstacle(Map::Up(player.position))) continue;
//             Map::Up(&player.position);
//         }
//         else if (task == "ml" || task == "move left" )
//         {
//             if (!Map::IsNotObstacle(Map::Left(player.position))) continue;
//             Map::Left(&player.position);
//         }
//         else if (task == "md" || task == "move down" )
//         {
//             if (!Map::IsNotObstacle(Map::Down(player.position))) continue;
//             Map::Down(&player.position);
//         }
//         else if (task == "mr" || task == "move right")
//         {
//             if (!Map::IsNotObstacle(Map::Right(player.position))) continue;
//             Map::Right(&player.position);
//         }

//         /// prep
//         else if (task == "pc" || task == "prepair chaos")
//             player.Prep(Player::SpellSource::CHAOS);
//         else if (task == "pw" || task == "prepair wrath")
//             player.Prep(Player::SpellSource::WRATH);
//         else if (task == "pd" || task == "prepair divinity")
//             player.Prep(Player::SpellSource::DIVINITY);

//         /// cast
//         else if (task == "c" || task == "cast")
//             player.Cast();

//         /// unknown command
//         else continue;

//         // enemy turn
//         for (auto& enemy : Enemy::enemies) {
//             enemy.Turn(player.position);
//             if (enemy.position == player.position) throw std::runtime_error("ERROR: GAME LOST");
//         }

//         // draw map
//         VisualDrawMap();
//         Visualizer::Out();
//     }
// }

// bool Player::Prep(Player::SpellSource newsource)
// {
//     if (newsource == Player::SpellSource::NONE) return false;

//     sources[0] = sources[1];
//     sources[1] = sources[2];
//     sources[2] = newsource;

//     return true;
// }

// bool Player::Cast()
// {
//     int amountC = 0;
//     int amountW = 0;
//     int amountD = 0;

//     for (int i = 0; i < 3; i++)
//         switch (sources[i])
//         {
//             case Player::SpellSource::CHAOS: amountC++; break;
//             case Player::SpellSource::WRATH: amountW++; break;
//             case Player::SpellSource::DIVINITY: amountD++; break;
//             default: return false;
//         }

//     if (amountC + amountW + amountD != 3) return false;

//     // ultimates Cases
//     else if (amountC == 3) {}
//     else if (amountW == 3) {}
//     else if (amountD == 3) {}

//     // get effect
//     int effect = amountC == EFF_AMOUNT ? ID_C
//                : amountW == EFF_AMOUNT ? ID_W
//                : amountD == EFF_AMOUNT ? ID_D
//                :                         ID_ERR;
//     int direction = amountC == DIR_AMOUNT ? ID_C
//                   : amountW == DIR_AMOUNT ? ID_W
//                   : amountD == DIR_AMOUNT ? ID_D
//                   :                         ID_ERR;
    
//     Log::Out("Casted " + std::to_string(effect) + " to " + std::to_string(direction));

//     auto cast_effect = 
//         effect == EFF_DIG ? [](int pos)
//         {
//             // enemy blocked
//             for (auto &enemy : Enemy::enemies)
//                 if (enemy.position == pos) return CASTBREAK;

//             // wall target
//             if (Map::Get(pos) == Map::TILE::WALL)
//             {
//                 Map::Get(pos) = Map::TILE::FLOOR;
//                 return CASTBREAK;
//             }

//             // nothing
//             return CASTCONTINUE;
//         }
//         // enemy
//         : effect == EFF_STUN ? [](int pos)
//         {
//             // wall block
//             if (Map::Get(pos) == Map::TILE::WALL) return CASTBREAK;

//             // enemy target
//             for (auto &enemy : Enemy::enemies)
//                 if (enemy.position == pos)
//                 {
//                     enemy.stunTime = 1;
//                     return CASTBREAK;
//                 }
            
//             // nothing
//             return CASTCONTINUE;
//         }
//         : [](int pos){ return CASTBREAK; };

//     // direction
//     if (direction == ID_ERR) return false;
//     else if (direction == DIR_CIRCLE)
//     {
//         int pos;
//         cast_effect(Map::Move(position, -1,  0)); // L
//         cast_effect(Map::Move(position,  1,  0)); // R
//         cast_effect(Map::Move(position,  0, -1)); // U
//         cast_effect(Map::Move(position,  0,  1)); // D
//         cast_effect(Map::Move(position, -1, -1)); // LU
//         cast_effect(Map::Move(position, -1,  1)); // LD
//         cast_effect(Map::Move(position,  1, -1)); // RU
//         cast_effect(Map::Move(position,  1,  1)); // RD
//     }
//     else if (direction == DIR_AXIS)
//     {
//         for ( // L
//             int limit = Map::MX/2, pos = position; 
//             limit > 0; 
//             limit ++
//         ) if (cast_effect(Map::Move(&pos, -1,  0)) == CASTBREAK) break;
//         for ( // R
//             int limit = Map::MX/2, pos = position; 
//             limit > 0; 
//             limit ++
//         ) if (cast_effect(Map::Move(&pos,  1,  0)) == CASTBREAK) break;
//         for ( // U
//             int limit = Map::MX/2, pos = position; 
//             limit > 0; 
//             limit ++
//         ) if (cast_effect(Map::Move(&pos,  0, -1)) == CASTBREAK) break;
//         for ( // D
//             int limit = Map::MX/2, pos = position; 
//             limit > 0; 
//             limit ++
//         ) if (cast_effect(Map::Move(&pos,  0,  1)) == CASTBREAK) break;
//     }
//     else if (direction == DIR_DIAGONAL)
//     {
//         for ( // LU
//             int limit = Map::MX/2, pos = position; 
//             limit > 0;
//             limit ++
//         ) if (cast_effect(Map::Move(&pos, -1, -1)) == CASTBREAK) break;
//         for ( // LD
//             int limit = Map::MX/2, pos = position; 
//             limit > 0; 
//             limit ++
//         ) if (cast_effect(Map::Move(&pos, -1,  1)) == CASTBREAK) break;
//         for ( // RU
//             int limit = Map::MX/2, pos = position; 
//             limit > 0; 
//             limit ++
//         ) if (cast_effect(Map::Move(&pos,  1, -1)) == CASTBREAK) break;
//         for ( // RD
//             int limit = Map::MX/2, pos = position; 
//             limit > 0; 
//             limit ++
//         ) if (cast_effect(Map::Move(&pos,  1,  1)) == CASTBREAK) break;
//     }

//     sources[0] = SpellSource::NONE;

//     return true;
// }

// bool Player::Move()
// {
//     return true;
// }
