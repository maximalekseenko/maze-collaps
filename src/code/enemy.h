#pragma once


#include "random.h"
#include <vector>
#include "map.h"


struct Enemy
{
    public:
        static std::vector<Enemy> enemies;

    public: // +++BEHAVIOR VARIABLES+++
        int health;
        int x, y;
        bool aleart = false;
        int tx, ty;
        std::string name;
        std::wstring visual;
        std::wstring visual_attk;
        std::wstring visual_idle;
        double difficulty;

    private:
        Enemy(std::string name, int health, std::wstring visual_idle, std::wstring visual_attk, double difficulty, int x, int y)
            : name(name), health(health), difficulty(difficulty),
              visual(visual_idle), visual_idle(visual_idle), visual_attk(visual_attk),
            x(x), y(y) {}

    public:
        void Turn(int tx, int ty, bool LOS, double difficulty_mod = 1.0)
        {
            // bool act = dist(gen) > difficulty;

            // if (aleart && !LOS && !act) aleart = false;
            // else if (act && !aleart && !LOS) Roam();
            // else if (aleart && act) Attack();
            // else { aleart = false;}
            
            // if (value > difficulty) return;
        }

    private:
        void Roam()
        {

        }
        void Attack()
        {

        }

    public: // +++CREATION AND MODIFICATION+++
        static bool Create(int x, int y, double difficulty_mod = 1.0)
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

};
std::vector<Enemy> Enemy::enemies;