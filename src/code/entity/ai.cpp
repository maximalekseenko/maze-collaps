#include "entity.h"
#include "player.h"
#include "enemy.h"

#include "../game.h"
#include "../lib/log.h"
#include "../lib/random.h"



bool IsNotObstacle(int __pos) {
    if (Game::game.current_map->Get(__pos) == Map::TILE::WALL) return false;
    for (auto __ent : Game::game.entities) if (__ent->position == __pos) return false;
    return true;
}

Enemy::Enemy(int __pos) : Entity(__pos)
{
    double value = Random::Get();
    this->type =
        value == 0   ? -1 :
        value < 0.40 ?  0 :
        value < 0.60 ?  1 :
        value < 0.80 ?  2 :
        value < 0.90 ?  3 :
        value < 0.95 ?  4 :
        value < 1    ?  5
                     : -1;
}
const char* Enemy::GetVisual() {
    return 
        type == -1 ? "❦" :
        type == 0 ? ( activated ? "♟" : "♙" ) :
        type == 1 ? ( activated ? "♞" : "♘" ) :
        type == 2 ? ( activated ? "♝" : "♗" ) :
        type == 3 ? ( activated ? "♜" : "♖" ) :
        type == 4 ? ( activated ? "♛" : "♕" ) :
        type == 5 ? ( activated ? "♚" : "♔" )
            : "?";
}



std::vector<int> GetMovesForEnemy(Enemy* __ent) {
    std::vector<int> possibleMoves;

    if (__ent->type == 1) {
        int pos;
        pos = Game::game.current_map->Move(__ent->position, -2, -1); // LLU
        if (IsNotObstacle(pos)) possibleMoves.push_back(pos);
        pos = Game::game.current_map->Move(__ent->position, -2,  1); // LLD
        if (IsNotObstacle(pos)) possibleMoves.push_back(pos);
        pos = Game::game.current_map->Move(__ent->position, -1, -2); // LUU
        if (IsNotObstacle(pos)) possibleMoves.push_back(pos);
        pos = Game::game.current_map->Move(__ent->position, -1,  2); // LDD
        if (IsNotObstacle(pos)) possibleMoves.push_back(pos);
        pos = Game::game.current_map->Move(__ent->position,  2, -1); // RRU
        if (IsNotObstacle(pos)) possibleMoves.push_back(pos);
        pos = Game::game.current_map->Move(__ent->position,  2,  1); // RRD
        if (IsNotObstacle(pos)) possibleMoves.push_back(pos);
        pos = Game::game.current_map->Move(__ent->position,  1, -2); // RUU
        if (IsNotObstacle(pos)) possibleMoves.push_back(pos);
        pos = Game::game.current_map->Move(__ent->position,  1,  2); // RDD
        if (IsNotObstacle(pos)) possibleMoves.push_back(pos);
    } else {

            int distlimit = __ent->type == 0 || __ent->type == 5 ? 1 : 10;
            bool move_hv = __ent->type == 0
                        || __ent->type == 3
                        || __ent->type == 4
                        || __ent->type == 5
                        || __ent->type == -1;
            bool move_d = __ent->type == 2
                        || __ent->type == 4
                        || __ent->type == 5
                        || __ent->type == -1;;

            if (move_hv) {
                for ( // L
                    int limit = 0, pos = __ent->position; 
                    limit < distlimit && IsNotObstacle(Game::game.current_map->Move(&pos, -1,  0)); 
                    limit ++
                ) possibleMoves.push_back(pos);
                for ( // R
                    int limit = 0, pos = __ent->position; 
                    limit < distlimit && IsNotObstacle(Game::game.current_map->Move(&pos,  1,  0)); 
                    limit ++
                ) possibleMoves.push_back(pos);
                for ( // U
                    int limit = 0, pos = __ent->position; 
                    limit < distlimit && IsNotObstacle(Game::game.current_map->Move(&pos,  0, -1)); 
                    limit ++
                ) possibleMoves.push_back(pos);
                for ( // D
                    int limit = 0, pos = __ent->position; 
                    limit < distlimit && IsNotObstacle(Game::game.current_map->Move(&pos,  0,  1)); 
                    limit ++
                ) possibleMoves.push_back(pos);
            }
            if (move_d) {
                for ( // LU
                    int limit = 0, pos = __ent->position; 
                    limit < distlimit && IsNotObstacle(Game::game.current_map->Move(&pos, -1, -1)); 
                    limit ++
                ) possibleMoves.push_back(pos);
                for ( // LD
                    int limit = 0, pos = __ent->position; 
                    limit < distlimit && IsNotObstacle(Game::game.current_map->Move(&pos, -1,  1)); 
                    limit ++
                ) possibleMoves.push_back(pos);
                for ( // RD
                    int limit = 0, pos = __ent->position; 
                    limit < distlimit && IsNotObstacle(Game::game.current_map->Move(&pos,  1, -1)); 
                    limit ++
                ) possibleMoves.push_back(pos);
                for ( // RD
                    int limit = 0, pos = __ent->position; 
                    limit < distlimit && IsNotObstacle(Game::game.current_map->Move(&pos,  1,  1)); 
                    limit ++
                ) possibleMoves.push_back(pos);
            }
    }
    return possibleMoves;
}
void Enemy::Turn() {

    if (!this->activated) {
        if (Random::Get() <= 0.2) {
            this->activated = true;
        }
    } else {

        std::vector<int> possibleMoves = GetMovesForEnemy(this);

        int bestMove = position;
        double bestWeight = -1;
        for (auto newPosition : possibleMoves)
        {

            // get weight of this move
            double posWeight = Random::Get() + std::sqrt(
                std::pow(std::min(
                    Game::game.current_map->X(position) - Game::game.current_map->X(Game::game.entities[0]->position),
                    Game::game.current_map->X(position) + Game::game.current_map->X(Game::game.entities[0]->position)) - Game::game.current_map->MX, 2)
                + std::pow(std::min(
                    Game::game.current_map->Y(position) - Game::game.current_map->Y(Game::game.entities[0]->position),
                    Game::game.current_map->Y(position) + Game::game.current_map->Y(Game::game.entities[0]->position)) - Game::game.current_map->MY, 2)
            );
            // is better?
            if (posWeight < bestWeight || bestWeight == -1)
            {
                bestWeight = posWeight;
                bestMove = newPosition;
            }
        }
        this->position = bestMove;

        Log::Out("Entity moved " + std::to_string(possibleMoves.size()));

        // reset
        this->activated = false;
    }

    std::vector<int> possibleMoves;
    // int pos;
    
        // for ( // L
        //     int limit = 0, pos = position; 
        //     limit < AIMOVEDISTANCE && Map::IsNotObstacle(Map::Move(&pos, -1,  0)); 
        //     limit ++
        // ) possibleMoves.push_back(pos);
        // for ( // R
        //     int limit = 0, pos = position; 
        //     limit < AIMOVEDISTANCE && Map::IsNotObstacle(Map::Move(&pos,  1,  0)); 
        //     limit ++
        // ) possibleMoves.push_back(pos);
        // for ( // U
        //     int limit = 0, pos = position; 
        //     limit < AIMOVEDISTANCE && Map::IsNotObstacle(Map::Move(&pos,  0, -1)); 
        //     limit ++
        // ) possibleMoves.push_back(pos);
        // for ( // D
        //     int limit = 0, pos = position; 
        //     limit < AIMOVEDISTANCE && Map::IsNotObstacle(Map::Move(&pos,  0,  1)); 
        //     limit ++
        // ) possibleMoves.push_back(pos);
        // for ( // LU
        //     int limit = 0, pos = position; 
        //     limit < AIMOVEDISTANCE && Map::IsNotObstacle(Map::Move(&pos, -1, -1)); 
        //     limit ++
        // ) possibleMoves.push_back(pos);
        // for ( // LD
        //     int limit = 0, pos = position; 
        //     limit < AIMOVEDISTANCE && Map::IsNotObstacle(Map::Move(&pos, -1,  1)); 
        //     limit ++
        // ) possibleMoves.push_back(pos);
        // for ( // RD
        //     int limit = 0, pos = position; 
        //     limit < AIMOVEDISTANCE && Map::IsNotObstacle(Map::Move(&pos,  1, -1)); 
        //     limit ++
        // ) possibleMoves.push_back(pos);
        // for ( // RD
        //     int limit = 0, pos = position; 
        //     limit < AIMOVEDISTANCE && Map::IsNotObstacle(Map::Move(&pos,  1,  1)); 
        //     limit ++
        // ) possibleMoves.push_back(pos);
}


const char* Player::GetVisual() {
    return "0";
}
void Player::Turn() {
    Log::Out("P");
}