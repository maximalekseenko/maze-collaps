#include "enemy.h"


#include "random.h"
#include <vector>
#include "map.h"




std::vector<Enemy> Enemy::enemies;

// +++BEHAVIOR VARIABLES+++


Enemy::Enemy(std::string name, int health, std::wstring visual_idle, std::wstring visual_attk, double difficulty, int x, int y)
    : name(name), health(health), difficulty(difficulty),
        visual(visual_idle), visual_idle(visual_idle), visual_attk(visual_attk),
    x(x), y(y) {}


void Enemy::Turn(int tx, int ty, bool LOS, double difficulty_mod)
{
    // bool act = dist(gen) > difficulty;

    // if (aleart && !LOS && !act) aleart = false;
    // else if (act && !aleart && !LOS) Roam();
    // else if (aleart && act) Attack();
    // else { aleart = false;}
    
    // if (value > difficulty) return;
}


void Enemy::Roam()
{

}
void Enemy::Attack()
{

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
