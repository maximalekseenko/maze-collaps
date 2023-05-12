#pragma once


#include <string>
#include <vector>


struct Enemy
{
    public:
        static std::vector<Enemy> enemies;

    public: // +++BEHAVIOR VARIABLES+++
        int health;
        int x, y;
        bool isChasing;
        bool isAttacking;
        int tx, ty;
        std::string name;
        std::wstring visual;
        std::wstring visual_attk;
        std::wstring visual_idle;
        double difficulty;

    private:
        Enemy(std::string name, int health, std::wstring visual_idle, std::wstring visual_attk, double difficulty, int x, int y);

    public:
        void Turn(int tx, int ty, double difficulty_mod = 1.0);
    
    private:
        void AttackStart();
        void AttackEnd();

    private:
        void Roam();
        void Attack(int tx, int ty);


    public: // +++CREATION AND MODIFICATION+++
        static bool Create(int x, int y, double difficulty_mod = 1.0);

};