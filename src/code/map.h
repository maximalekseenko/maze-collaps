#pragma once
#include <string>
#include <tuple>


struct Map
{
    public: // +++TILE+++
        enum TILE
        {
            WALL    = 1052688,
            FLOOR   = 10526880,
            SPAWNER = 1052832,
            TRAP    = 1089552,
        };

    public:
        Map();
        Map(std::string path);
        ~Map();

    private: // +++MEMBERS+++
        static TILE* map;

    public: // +++READONLY* MEMBERS+++
        int MX, MY, MI;
        std::string name;
        std::string path;
        double difficulty;


    public: // +++LOAD FUNCTIONS+++
        bool Load(std::string fileName);

    public: // +++ACCESS FUNCTIONS+++
        TILE& Get(int pos);
        int Move(int* fromPos, int dX, int dY);
        int Move(int fromPos, int dX, int dY);
        int Up(int* fromPos);
        int Up(int fromPos);
        int Down(int* fromPos);
        int Down(int fromPos);
        int Left(int* fromPos);
        int Left(int fromPos);
        int Right(int* fromPos);
        int Right(int fromPos);
        int Fix(int* fromPos);
        int Fix(int fromPos);
        int FixX(int* x);
        int FixX(int x);
        int FixY(int* y);
        int FixY(int y);
        int X(int position);
        int Y(int position);
        int I(int x, int y);


    public: // +++GEOMETRY FUNCTIONS+++
        bool IsNotObstacle(int pos);
        bool IsLineOfSight(int fromPos, int toPos);
};