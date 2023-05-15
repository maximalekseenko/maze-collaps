#include "enemy.h"


#include "random.h"
#include "map.h"
#include "log.h"


#include <vector>
#include <tuple>


#define MOVELIMIT 3


std::vector<Enemy> Enemy::enemies;




Enemy::Enemy(std::string name, int health, std::wstring visual_idle, std::wstring visual_attk, double difficulty, int position)
    : name(name), health(health), difficulty(difficulty),
        visual(visual_idle), visual_idle(visual_idle), visual_attk(visual_attk),
    position(position){}


void Enemy::Turn(int targetPosition, double difficulty_mod)
{
    bool isTurning = Random::Get() - (0.2 * isChasing) < difficulty * difficulty_mod;
    bool isInLOS = Map::IsLineOfSight(this->position, targetPosition);


    // Attack
    if (isAttacking){
        if (isChasing) Attack(targetPosition);
        else Attack(this->position);
    }
    else if (isTurning) AttackStart();


    // LOS
    if (isInLOS) isChasing = true;
    else if (isChasing && !isTurning) isChasing = false;
}

void Enemy::AttackStart()
{
    isAttacking = true;
    visual = visual_attk;
}
void Enemy::AttackEnd()
{
    isAttacking = false;
    visual = visual_idle;
}
void Enemy::Attack(int targetPosition)
{
    if (name == "k" && Random::Get() < 0.2) 
    {
        int direction = Random::Get() * 4;
        switch (direction)
        {
        case 0:
            if (Map::IsNotObstacle(Map::Move(position,  0,  1))) Enemy::Create(Map::Move(position,  0,  1), 0.2);
            return;
        
        case 1:
            if (Map::IsNotObstacle(Map::Move(position,  0, -1))) Enemy::Create(Map::Move(position,  0, -1), 0.2);
            return;
        
        case 2:
            if (Map::IsNotObstacle(Map::Move(position,  1,  0))) Enemy::Create(Map::Move(position,  1,  0), 0.2);
            return;
        
        case 3:
            if (Map::IsNotObstacle(Map::Move(position, -1,  0))) Enemy::Create(Map::Move(position, -1,  0), 0.2);
            return;
        
        default:
            Log::Out("--ERR: King spawn direction");
            return;
        }
    }

    std::vector<int> possibleMoves = GetMoves(name, position);
    int bestMove = position;
    double bestWeight = -1;

    for (auto newPosition : possibleMoves)
    {
        double posWeight = Random::Get()
             + (isChasing ? GetDepth(newPosition, targetPosition, MOVELIMIT) : 0);
        if (posWeight < bestWeight || bestWeight == -1)
        {
            bestWeight = posWeight;
            bestMove = newPosition;
        }
    }
    
    position = bestMove;

    AttackEnd();
}
double Enemy::GetDepth(int position, int targetPosition, int moveLimit)
{

    if (position == targetPosition) return 0;

    // limit -> break recursion and return distance to target
    if (moveLimit <= 0) return 
            std::min(
                abs(Map::X(position) - Map::X(targetPosition)),
                abs(Map::X(position) + Map::X(targetPosition) - Map::MX)
            ) + std::min(
                abs(Map::Y(position) - Map::Y(targetPosition)),
                abs(Map::Y(position) + Map::Y(targetPosition) - Map::MY)
            );

    int bestDepth = -1;

    // get possible move
    std::vector<int> possibleMoves = GetMoves(name, position);

    // find best depth
    for (auto newPosition : possibleMoves)
    {
    
        // if target -> break recursion
        if (newPosition == targetPosition) return 1;

        // get that depth
        int newPosDepth = GetDepth(newPosition, targetPosition, moveLimit -1);
        if (newPosDepth < bestDepth || bestDepth == -1)
            bestDepth = newPosDepth;
    }
    
    // return
    bestDepth ++;
    return bestDepth;
}


// +++CREATION AND MODIFICATION+++
bool Enemy::Create(int position, double difficulty_mod)
{
    if (!Map::IsNotObstacle(position)) return false;

    double value = Random::Get();

    enemies.push_back(
        value == 0   ? Enemy("d", 5, L"❦", L"❦", 0.9,                  position) :
        value < 0.40 ? Enemy("p", 1, L"♙", L"♟", 0.1 * difficulty_mod, position) :
        value < 0.60 ? Enemy("n", 1, L"♘", L"♞", 0.1 * difficulty_mod, position) :
        value < 0.80 ? Enemy("b", 1, L"♗", L"♝", 0.2 * difficulty_mod, position) :
        value < 0.90 ? Enemy("r", 1, L"♖", L"♜", 0.2 * difficulty_mod, position) :
        value < 0.95 ? Enemy("q", 1, L"♕", L"♛", 0.1 * difficulty_mod, position) :
        value < 1    ? Enemy("k", 1, L"♔", L"♚", 0.1 * difficulty_mod, position) :
                       Enemy("e", -1,L"#", L"#", -1,                   position)
    );

    return true;
}




std::vector<int> Enemy::GetMoves(std::string name, int position){
    std::vector<int> possibleMoves;
    if (name == "p" || name == "k") // one step horizontal
    {
        int pos;
        pos = Map::Move(position,  0,  1); // D
        if (Map::IsNotObstacle(pos)) possibleMoves.push_back(pos);
        pos = Map::Move(position,  0, -1); // U
        if (Map::IsNotObstacle(pos)) possibleMoves.push_back(pos);
        pos = Map::Move(position,  1,  0); // R
        if (Map::IsNotObstacle(pos)) possibleMoves.push_back(pos);
        pos = Map::Move(position, -1,  0); // L
        if (Map::IsNotObstacle(pos)) possibleMoves.push_back(pos);
    }
    if (name == "k") // one step diadonal
    {
        int pos;
        pos = Map::Move(position,  1,  1); // RD
        if (Map::IsNotObstacle(pos)) possibleMoves.push_back(pos);
        pos = Map::Move(position,  1, -1); // RU
        if (Map::IsNotObstacle(pos)) possibleMoves.push_back(pos);
        pos = Map::Move(position, -1,  1); // LD
        if (Map::IsNotObstacle(pos)) possibleMoves.push_back(pos);
        pos = Map::Move(position, -1, -1); // LU
        if (Map::IsNotObstacle(pos)) possibleMoves.push_back(pos);
    }
    if (name == "r" || name == "q") // horizontal
    {
        for (
            int limit = Map::MX/2, pos = position; 
            limit > 0 && Map::IsNotObstacle(Map::Move(&pos, -1,  0)); 
            limit ++
        ) possibleMoves.push_back(pos);
        for (
            int limit = Map::MX/2, pos = position; 
            limit > 0 && Map::IsNotObstacle(Map::Move(&pos,  1,  0)); 
            limit ++
        ) possibleMoves.push_back(pos);
        for (
            int limit = Map::MX/2, pos = position; 
            limit > 0 && Map::IsNotObstacle(Map::Move(&pos,  0,  1)); 
            limit ++
        ) possibleMoves.push_back(pos);
        for (
            int limit = Map::MX/2, pos = position; 
            limit > 0 && Map::IsNotObstacle(Map::Move(&pos,  0, -1)); 
            limit ++
        ) possibleMoves.push_back(pos);
    }
    if (name == "b" || name == "q") // diagonal
    {
        for (
            int limit = Map::MX/2, pos = position; 
            limit > 0 && Map::IsNotObstacle(Map::Move(&pos, -1,  1)); 
            limit ++
        ) possibleMoves.push_back(pos);
        for (
            int limit = Map::MX/2, pos = position; 
            limit > 0 && Map::IsNotObstacle(Map::Move(&pos,  1, -1)); 
            limit ++
        ) possibleMoves.push_back(pos);
        for (
            int limit = Map::MX/2, pos = position; 
            limit > 0 && Map::IsNotObstacle(Map::Move(&pos,  1,  1)); 
            limit ++
        ) possibleMoves.push_back(pos);
        for (
            int limit = Map::MX/2, pos = position; 
            limit > 0 && Map::IsNotObstacle(Map::Move(&pos, -1, -1)); 
            limit ++
        ) possibleMoves.push_back(pos);
    }
    if (name == "n") // knight
    {
        int pos;
        pos = Map::Move(position,  2,  1); // RRD
        if (Map::IsNotObstacle(pos)) possibleMoves.push_back(pos);
        pos = Map::Move(position,  2, -1); // RRU
        if (Map::IsNotObstacle(pos)) possibleMoves.push_back(pos);
        pos = Map::Move(position, -2,  1); // LLD
        if (Map::IsNotObstacle(pos)) possibleMoves.push_back(pos);
        pos = Map::Move(position, -2, -1); // LLU
        if (Map::IsNotObstacle(pos)) possibleMoves.push_back(pos);
        pos = Map::Move(position,  1,  2); // DDR
        if (Map::IsNotObstacle(pos)) possibleMoves.push_back(pos);
        pos = Map::Move(position, -1,  2); // DDL
        if (Map::IsNotObstacle(pos)) possibleMoves.push_back(pos);
        pos = Map::Move(position,  1, -2); // UUR
        if (Map::IsNotObstacle(pos)) possibleMoves.push_back(pos);
        pos = Map::Move(position, -1, -2); // UUL
        if (Map::IsNotObstacle(pos)) possibleMoves.push_back(pos);
    }

    return possibleMoves;
}