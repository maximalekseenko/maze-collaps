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


    private: // +++MEMBERS+++
        static TILE* map;

    public: // +++READONLY* MEMBERS+++
        static int MX, MY, MI;
        static std::string mapName;
        static int mapId;
        static double difficulty;


    public: // +++LOAD FUNCTIONS+++
        static bool Load(std::string fileName);
        static bool Load(int mapId);
        static bool LoadNext();


    public: // +++ACCESS FUNCTIONS+++
        static TILE& Get(int pos);
        static int Move(int* fromPos, int dX, int dY);
        static int Move(int fromPos, int dX, int dY);
        static int Up(int* fromPos);
        static int Up(int fromPos);
        static int Down(int* fromPos);
        static int Down(int fromPos);
        static int Left(int* fromPos);
        static int Left(int fromPos);
        static int Right(int* fromPos);
        static int Right(int fromPos);
        static int Fix(int* fromPos);
        static int Fix(int fromPos);
        static int FixX(int* x);
        static int FixX(int x);
        static int FixY(int* y);
        static int FixY(int y);
        static int X(int position);
        static int Y(int position);
        static int I(int x, int y);


    public: // +++GEOMETRY FUNCTIONS+++
        static bool IsNotObstacle(int pos);
        static bool IsLineOfSight(int fromPos, int toPos);
};