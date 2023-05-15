#pragma once


#include <string>
#include <vector>
#include <map>
#include <functional>



struct Enemy
{
    public:
        enum Name
        {
            DRAGON = 0,
            PAWN   = 1,
            KNIGHT = 2,
            BISHOP = 3,
            ROOK   = 4,
            QUEEN  = 5,
            KING   = 6
        };
        struct EnemyData
        {  
            const std::wstring visual_idle;
            const std::wstring visual_attk;
            const std::function<std::vector<int>(int)> GetMoves;
        };
        static const std::map<Name, EnemyData> ENEMYDATAS;
        static std::vector<Enemy> enemies;

    public: // +++BEHAVIOR VARIABLES+++
        Name name;
        double difficulty;
        int position;
        bool isChasing = false;
        bool isAttacking = false;

    private:
        Enemy(Name name, int position, double difficulty = 1.0);


    public: // +++COMMON USE+++
        std::wstring GetVisual();
        void Turn(int targetPosition, double difficulty_mod = 1.0);
    

    private: // +++TURN SUPPORT FUNCTION+++
        void Attack(int targetPos);
        double GetDepth(int position, int targetPosition, int moveLimit);


    public: // +++CREATION AND MODIFICATION+++
        static bool Create(int position, double difficulty_mod = 1.0);

};