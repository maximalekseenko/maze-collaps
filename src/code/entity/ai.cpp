#include "entity.h"
#include "player.h"
#include "enemy.h"

#include "../game.h"
#include "../lib/log.h"
#include "../lib/random.h"


// TEMP
#include <ncurses.h>


bool IsNotObstacle(int __pos) {
    if (Game::game.current_map->Get(__pos) == Map::TILE::WALL) return false;
    for (auto __ent : Game::game.entities) if (__pos == __ent->position && __ent->id != 0) return false;
    return true;
}

Enemy::Enemy(int __pos) : Entity(__pos)
{
    double value = Random::Get();
    this->type =
        value < 0.40 ?  0 :
        value < 0.60 ?  1 :
        value < 0.80 ?  2 :
        value < 0.90 ?  3 :
        value < 0.95 ?  4 :
        value < 0.001?  5
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



std::vector<int> GetMovesForEnemy(int __type, int __pos) {
    std::vector<int> possibleMoves;

    if (__type == 1) {
        int pos;
        pos = Game::game.current_map->Move(__pos, -2, -1); // LLU
        if (IsNotObstacle(pos)) possibleMoves.push_back(pos);
        pos = Game::game.current_map->Move(__pos, -2,  1); // LLD
        if (IsNotObstacle(pos)) possibleMoves.push_back(pos);
        pos = Game::game.current_map->Move(__pos, -1, -2); // LUU
        if (IsNotObstacle(pos)) possibleMoves.push_back(pos);
        pos = Game::game.current_map->Move(__pos, -1,  2); // LDD
        if (IsNotObstacle(pos)) possibleMoves.push_back(pos);
        pos = Game::game.current_map->Move(__pos,  2, -1); // RRU
        if (IsNotObstacle(pos)) possibleMoves.push_back(pos);
        pos = Game::game.current_map->Move(__pos,  2,  1); // RRD
        if (IsNotObstacle(pos)) possibleMoves.push_back(pos);
        pos = Game::game.current_map->Move(__pos,  1, -2); // RUU
        if (IsNotObstacle(pos)) possibleMoves.push_back(pos);
        pos = Game::game.current_map->Move(__pos,  1,  2); // RDD
        if (IsNotObstacle(pos)) possibleMoves.push_back(pos);
    } else {

            int distlimit = __type == 0 || __type == 5 ? 1 : 10;
            bool move_hv = __type == 0
                        || __type == 3
                        || __type == 4
                        || __type == 5
                        || __type == -1;
            bool move_d = __type == 2
                        || __type == 4
                        || __type == 5
                        || __type == -1;;

            if (move_hv) {
                for ( // L
                    int limit = 0, pos = __pos; 
                    limit < distlimit && IsNotObstacle(Game::game.current_map->Move(&pos, -1,  0)); 
                    limit ++
                ) possibleMoves.push_back(pos);
                for ( // R
                    int limit = 0, pos = __pos; 
                    limit < distlimit && IsNotObstacle(Game::game.current_map->Move(&pos,  1,  0)); 
                    limit ++
                ) possibleMoves.push_back(pos);
                for ( // U
                    int limit = 0, pos = __pos; 
                    limit < distlimit && IsNotObstacle(Game::game.current_map->Move(&pos,  0, -1)); 
                    limit ++
                ) possibleMoves.push_back(pos);
                for ( // D
                    int limit = 0, pos = __pos; 
                    limit < distlimit && IsNotObstacle(Game::game.current_map->Move(&pos,  0,  1)); 
                    limit ++
                ) possibleMoves.push_back(pos);
            }
            if (move_d) {
                for ( // LU
                    int limit = 0, pos = __pos; 
                    limit < distlimit && IsNotObstacle(Game::game.current_map->Move(&pos, -1, -1)); 
                    limit ++
                ) possibleMoves.push_back(pos);
                for ( // LD
                    int limit = 0, pos = __pos; 
                    limit < distlimit && IsNotObstacle(Game::game.current_map->Move(&pos, -1,  1)); 
                    limit ++
                ) possibleMoves.push_back(pos);
                for ( // RD
                    int limit = 0, pos = __pos; 
                    limit < distlimit && IsNotObstacle(Game::game.current_map->Move(&pos,  1, -1)); 
                    limit ++
                ) possibleMoves.push_back(pos);
                for ( // RD
                    int limit = 0, pos = __pos; 
                    limit < distlimit && IsNotObstacle(Game::game.current_map->Move(&pos,  1,  1)); 
                    limit ++
                ) possibleMoves.push_back(pos);
            }
    }
    return possibleMoves;
}

double GetPositionWeight(int __type, int __pos, int __tpos, int __mlim)
{
    // target -> return mlim left
    if (__pos == __tpos) return __mlim;

    // mlim -> weight depends on distance to target
    if (__mlim <= 0) return -sqrt(
             + std::pow(std::min(
                abs(Game::game.current_map->X(__pos) - Game::game.current_map->X(__tpos)),
                Game::game.current_map->MX - abs(Game::game.current_map->X(__pos) - Game::game.current_map->X(__tpos))), 2)
             + std::pow(std::min(
                abs(Game::game.current_map->Y(__pos) - Game::game.current_map->Y(__tpos)),
                Game::game.current_map->MY - abs(Game::game.current_map->Y(__pos) - Game::game.current_map->Y(__tpos))), 2)
            );

    double _bestWeight = -9999;
    // find best next move
    for (auto _newPos : GetMovesForEnemy(__type, __pos))
    {   
        // get new pos weight
        double _newPosWeight = GetPositionWeight(__type, _newPos, __tpos, __mlim -1);
        if (_newPosWeight > _bestWeight || _bestWeight == -9999)
            _bestWeight = _newPosWeight;
    }
    
    // return
    return _bestWeight;
}

void Enemy::Turn() {

    if (!this->activated) {
        if (Random::Get() <= 0.2) {
            this->activated = true;
        }
    } else {

        Log::Out("--LOG: Enemy::Attack is called by "
            + std::to_string(type)
            + " from " + std::to_string(position)
            + " to " + std::to_string(Game::game.entities[0]->position));

        int bestMove = position;
        double bestWeight = -9999;
            Log::Out("---" + std::to_string(type));
        for (auto newPosition : GetMovesForEnemy(this->type, this->position))
        {
            // get weight of this move
            double posWeight = Random::Get() + GetPositionWeight(this->type, newPosition, Game::game.entities[0]->position, 3);
            // is better?
            Log::Out(std::to_string(posWeight) + 
                " [" + std::to_string(Game::game.current_map->X(newPosition)) + " " + std::to_string(Game::game.current_map->Y(newPosition)) + "] to" +
                " [" + std::to_string(Game::game.current_map->X(Game::game.entities[0]->position)) + " " + std::to_string(Game::game.current_map->Y(Game::game.entities[0]->position)) + "]"
            );
            if (posWeight > bestWeight || bestWeight == -9999)
            {
                bestWeight = posWeight;
                bestMove = newPosition;
            }
        }
        this->position = bestMove;

        // reset
        this->activated = false;
    }
}


const char* Player::GetVisual() {
    return "0";
}
void Player::Turn() {
    Log::Out(std::to_string(id));
    getch();
}