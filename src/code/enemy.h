#pragma once


#include "random.h"


struct Enemy
{
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

    public:
        static Enemy Create(int x, int y, double difficulty_mod = 1.0)
        {
            double value = Random::Get();

            if      (value == 0)   return Enemy("d", 5, L"❦", L"❦", 0.9,                  x, y);
            else if (value < 0.40) return Enemy("p", 1, L"♙", L"♟", 0.1 * difficulty_mod, x, y);
            else if (value < 0.60) return Enemy("k", 1, L"♘", L"♞", 0.1 * difficulty_mod, x, y);
            else if (value < 0.80) return Enemy("b", 1, L"♗", L"♝", 0.2 * difficulty_mod, x, y);
            else if (value < 0.90) return Enemy("r", 1, L"♖", L"♜", 0.2 * difficulty_mod, x, y);
            else if (value < 0.95) return Enemy("q", 1, L"♕", L"♛", 0.1 * difficulty_mod, x, y);
            else if (value < 1   ) return Enemy("k", 1, L"♔", L"♚", 0.1 * difficulty_mod, x, y);
            else                   return Enemy("e", -1,L"#", L"#", -1, x, y);
        }

};
