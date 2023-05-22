#pragma once


#include <string>
#include <vector>
#include <map>
#include <functional>

#include "player.h"


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
            const char* visual_idle;
            const char* visual_attk;
            const Player::Element Type;
            const std::function<std::vector<int>(int)> GetMoves;
        };
        static const std::map<Name, EnemyData> ENEMYDATAS;
        static std::vector<Enemy> enemies;
        static int lastId;

    public: // +++BEHAVIOR VARIABLES+++
        int id;
        Name name;
        double difficulty;
        int position;
        int stunTime = 0;
        bool isChasing = false;
        bool isAttacking = false;

    private:
        Enemy(Name name, int position, double difficulty = 1.0);


    public: // +++COMMON USE+++
        const char* GetVisual();
        void Turn(int targetPosition, double difficulty_mod = 1.0);
        bool Kill(Player::Element);
    

    private: // +++TURN SUPPORT FUNCTION+++
        void Attack(int targetPos);
        double GetDepth(int position, int targetPosition, int moveLimit);


    public: // +++CREATION AND MODIFICATION+++
        static bool Create(int position, double difficulty_mod = 1.0);

};