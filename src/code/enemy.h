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
        Enemy(std::string name, int health, std::wstring visual_idle, std::wstring visual_attk, double difficulty, int x, int y);

    public:
        void Turn(int tx, int ty, bool LOS, double difficulty_mod = 1.0);
    private:
        void Roam();
        void Attack();


    public: // +++CREATION AND MODIFICATION+++
        static bool Create(int x, int y, double difficulty_mod = 1.0);

};