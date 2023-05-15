#pragma once


#include <string>
#include <vector>

// enemies.push_back(
//         value == 0   ? Enemy("d", 5, L"❦", L"❦", 0.9,                  position) :
//         value < 0.40 ? Enemy("p", 1, L"♙", L"♟", 0.1 * difficulty_mod, position) :
//         value < 0.60 ? Enemy("n", 1, L"♘", L"♞", 0.1 * difficulty_mod, position) :
//         value < 0.80 ? Enemy("b", 1, L"♗", L"♝", 0.2 * difficulty_mod, position) :
//         value < 0.90 ? Enemy("r", 1, L"♖", L"♜", 0.2 * difficulty_mod, position) :
//         value < 0.95 ? Enemy("q", 1, L"♕", L"♛", 0.1 * difficulty_mod, position) :
//         value < 1    ? Enemy("k", 1, L"♔", L"♚", 0.1 * difficulty_mod, position) :
//                        Enemy("e", -1,L"#", L"#", -1,                   position)
//     );
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
        static std::vector<Enemy> enemies;
        static std::tuple<Name, std::wstring, std::wstring,std::wstring> enemiyData[];

    public: // +++BEHAVIOR VARIABLES+++
        int health;
        int position;
        bool isChasing = false;
        bool isAttacking = false;
        std::string name;
        std::wstring visual, visual_attk, visual_idle;
        double difficulty;

    private:
        Enemy(std::string name, int health, std::wstring visual_idle, std::wstring visual_attk, double difficulty, int position);

    public:
        void Turn(int targetPosition, double difficulty_mod = 1.0);
    
    private:
        void AttackStart();
        void AttackEnd();

    private:
        void Attack(int targetPos);
        double GetDepth(int position, int targetPosition, int moveLimit);

    public: // +++CREATION AND MODIFICATION+++
        static bool Create(int position, double difficulty_mod = 1.0);


        static std::vector<int> GetMoves(std::string name, int position);

};