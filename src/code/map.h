#pragma once
#include <string>


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
        static int MX, MY;
        static std::string mapName;
        static int mapId;
        static double difficulty;


    public: // +++LOAD FUNCTIONS+++
        static void Load(std::string fileName);
        static void Load(int mapId);
        static void LoadNext();


    public: // +++GEOMETRY FUNCTIONS+++

        static TILE& Get(int x, int y);


        static bool IsNotObstacle(int x, int y);
        static void Fix(int &x, int &y);

        static bool Move(int &x, int &y, int dx, int dy);

        static bool IsLineOfSight(int x1, int y1, int x2, int y2);

};