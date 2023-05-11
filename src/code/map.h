#pragma once
#include <string>
#include <tuple>
#include <fstream>


#include "overlappingwave.h"


#define GENTRYLIMIT 10


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
        static void Load(std::string fileName)
        {
            auto generator = OverlappingWave(
                "data/" + fileName + ".png", 3, Map::MX, Map::MY, 
                true, true, 8, false, Model::Heuristic::Entropy);
                
            // generate
            int trys = GENTRYLIMIT;
            for(; trys != 0; trys --) if (generator.Run(-1, -1)) break;
            if (trys == 0) throw std::runtime_error("LoadLevel trys limit reached");

            // set map
            if (Map::map != nullptr) delete[] Map::map;

            int* result = generator.Result();
            Map::map = new TILE[MX * MY];
            for (int i = 0; i < MX * MY; i ++)
                Map::map[i] = static_cast<TILE>(result[i]);
            delete[] result;
        }

        static void Load(int mapId)
        {
            Map::mapId = mapId;
            switch (mapId)
            {
                case 0: Load("hostilecave"); break;
                case 1: Load("theentrance"); break;
                default: break;
            }
        }
        static void LoadNext() { Load(mapId + 1); }


    public: // +++GEOMETRY FUNCTIONS+++

        static TILE& Get(int x, int y)
        {
            Fix(x, y);
            return map[x + y * MX];
        }


        static bool IsNotObstacle(int x, int y)
        {
            if (Get(x, y) == TILE::WALL) return false;
            // for (auto enemy : enemies) if (enemy.x == x && enemy.y == y) return false;
            return true;
        }

        static void Fix(int &x, int &y)
        {
            if (x >= MX) x %= MX;
            else if (x < 0) x = MX - (abs(x) % MX);

            if (y >= MY) y %= MY;
            else if (y < 0) y = MY - (abs(y) % MY);
        }

        static bool Move(int &x, int &y, int dx, int dy)
        {
            if (IsNotObstacle(x + dx, y + dy) && IsLineOfSight(x, y, x + dx, y + dy))
            {
                x += dx;
                y += dy;
                return true;
            }

            return false;
        }

        static bool IsLineOfSight(int x1, int y1, int x2, int y2)
        {
            // fix fo recursive
            if (abs(x2 - x1 + 1) > MX / 2)
            {
                if (x2 < x1) x2 += MX;
                else         x2 -= MX;
            }

            if (abs(y2 - y1 + 1) > MY / 2)
            {
                if (y2 < y1) y2 += MY;
                else         y2 -= MY;
            }

            // find obstacles
            int len = std::max(abs(x2 - x1), abs(y2 - y1));
            for (int i = 0; i < len; i ++)
            {
                double step = double(i) / len;
                int x = (int)(MX + round(x1 * (1.0 - step) + x2 * step)) % MX;
                int y = (int)(MY + round(y1 * (1.0 - step) + y2 * step)) % MY;
                
                if (map[x + y * MX] == 1052688) return false;
            }

            // no obstacles found
            return true;
        }

};

int Map::mapId = -1;
Map::TILE* Map::map = nullptr;
int Map::MX = 32, Map::MY = 16;