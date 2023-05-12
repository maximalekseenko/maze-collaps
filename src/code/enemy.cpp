#include "enemy.h"


#include "random.h"
#include "map.h"
#include "log.h"


#include <vector>



std::vector<Enemy> Enemy::enemies;




Enemy::Enemy(std::string name, int health, std::wstring visual_idle, std::wstring visual_attk, double difficulty, int x, int y)
    : name(name), health(health), difficulty(difficulty),
        visual(visual_idle), visual_idle(visual_idle), visual_attk(visual_attk),
    x(x), y(y) {}


void Enemy::Turn(int tx, int ty, double difficulty_mod)
{
    bool isTurning = Random::Get() - (0.2 * isChasing) < difficulty * difficulty_mod;
    bool isInLOS = Map::IsLineOfSight(x, y, tx, ty);


    Log::Out(std::to_string(x) + '-' + std::to_string(y) + ' '
           + std::to_string(isTurning) + ' ' 
           + std::to_string(isInLOS) + ' '
           + std::to_string(isAttacking) + ' ' 
           + std::to_string(isChasing));
        

    // Attack
    if (isAttacking)
        if (isChasing) Attack(tx, ty);
        else Attack(x, y);
    else if (isTurning) AttackStart();


    // LOS
    if (isInLOS) isChasing = true;
    else if (isChasing && !isTurning) isChasing = false;


    Log::Out(std::to_string(x) + '-' + std::to_string(y) + ' '
           + std::to_string(isTurning) + ' ' 
           + std::to_string(isInLOS) + ' '
           + std::to_string(isAttacking) + ' ' 
           + std::to_string(isChasing));
}

void Enemy::AttackStart()
{
    isAttacking = true;
    visual = visual_attk;
    Log::Out("Attack start" + std::to_string(isAttacking));
}
void Enemy::AttackEnd()
{
    isAttacking = false;
    visual = visual_idle;
    Log::Out("Attack end");
}


void Enemy::Roam()
{
    AttackEnd();
}

void Enemy::Attack(int tx, int ty)
{
    Log::Out("Attack!");

    AttackEnd();
}

// +++CREATION AND MODIFICATION+++
bool Enemy::Create(int x, int y, double difficulty_mod)
{
    if (!Map::IsNotObstacle(x, y)) return false;

    double value = Random::Get();

    enemies.push_back(
        value == 0   ? Enemy("d", 5, L"❦", L"❦", 0.9,                  x, y) :
        value < 0.40 ? Enemy("p", 1, L"♙", L"♟", 0.1 * difficulty_mod, x, y) :
        value < 0.60 ? Enemy("k", 1, L"♘", L"♞", 0.1 * difficulty_mod, x, y) :
        value < 0.80 ? Enemy("b", 1, L"♗", L"♝", 0.2 * difficulty_mod, x, y) :
        value < 0.90 ? Enemy("r", 1, L"♖", L"♜", 0.2 * difficulty_mod, x, y) :
        value < 0.95 ? Enemy("q", 1, L"♕", L"♛", 0.1 * difficulty_mod, x, y) :
        value < 1    ? Enemy("k", 1, L"♔", L"♚", 0.1 * difficulty_mod, x, y) :
                        Enemy("e", -1,L"#", L"#", -1, x, y)
    );

    return true;
}
