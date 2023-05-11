#pragma once



#include <tuple>
#include <iostream>
#include <string>
#include <sstream>  
#include <map>
#include <vector>

#include "overlappingwave.h"
#include "player.h"
#include "enemy.h"

#include "visualizer.h"




struct Game
{
    public: // ++MAP RELATED VARIABLES+++
        int* map = nullptr;
        int MX = 32, MY = 16;
        std::string mapName;
        int mapId;
        double difficulty;

    public:
        Player player;
        std::vector<Enemy> enemies;


    std::string command, arg1, arg2;

    private: // +++STATIC+++
        const static std::tuple<std::string, std::string> MAPS[];
        const static std::map<int, const std::wstring> TILES;
        const static int GENTRYLIMIT;


    public: // +++LOAD MAP+++
        void LoadMap(int mapId)
        {
            this->mapId = mapId;
            this->difficulty = 1 + 0.1 * mapId;
            this->mapName = std::get<0>(MAPS[mapId]);

            MapGenerate();

            MapInit();

            VisualBackgroudMap();

            VisualDrawMap();

            Visualizer::Out();
        }

    
    private: // +++LOAD MAP+++
        void MapGenerate()
        {
            // generate
            auto generator = OverlappingWave(std::get<1>(MAPS[mapId]), 3, MX, MY, true, true, 8, false, Model::Heuristic::Entropy);
            int trys = GENTRYLIMIT;
            for(; trys != 0; trys --) if (generator.Run(-1, -1)) break;
            if (trys == 0) throw std::runtime_error("LoadLevel trys limit reached");

            // set map
            if (this->map != nullptr) delete[] this->map;
            this->map = generator.Result();
        }

        void MapInit()
        {

            bool isPlayerCreated = false;
            for (int y = 0; y < MY; y ++)
                for (int x = 0; x < MX; x ++)

                    // if spawner
                    if (map[x + y * MX] == 1052832)
                    {
                        // spawn player
                        if (!isPlayerCreated) 
                        {
                            isPlayerCreated = true;
                            player = Player(x, y);
                        }

                        // spawn enemy
                        else enemies.push_back(Enemy::Create(x, y));
                    }
        }


    void VisualBackgroudMap()
    {
        Visualizer::Write(0,  0, L"----------------------------------------------------------------------------------------------------");
        Visualizer::Write(0,  1, L"|                                                                                                  |");
        Visualizer::Write(0,  2, L"|                                                                                                  |");
        Visualizer::Write(0,  3, L"|                                                                                                  |");
        Visualizer::Write(0,  4, L"||--------------------------------|              Spell 1                     Spell 2               |");
        Visualizer::Write(0,  5, L"||................................|       |--------------------|      |--------------------|       |");
        Visualizer::Write(0,  6, L"||................................|       |                    |      |                    |       |");
        Visualizer::Write(0,  7, L"||................................|       |                    |      |                    |       |");
        Visualizer::Write(0,  8, L"||................................|       |                    |      |                    |       |");
        Visualizer::Write(0,  9, L"||................................|       |                    |      |                    |       |");
        Visualizer::Write(0, 10, L"||................................|       |                    |      |                    |       |");
        Visualizer::Write(0, 11, L"||................................|       |--------------------|      |--------------------|       |");
        Visualizer::Write(0, 12, L"||................................|              Spell 3                       Item                |");
        Visualizer::Write(0, 13, L"||................................|       |--------------------|      |--------------------|       |");
        Visualizer::Write(0, 14, L"||................................|       |                    |      |                    |       |");
        Visualizer::Write(0, 15, L"||................................|       |                    |      |                    |       |");
        Visualizer::Write(0, 16, L"||................................|       |                    |      |                    |       |");
        Visualizer::Write(0, 17, L"||................................|       |                    |      |                    |       |");
        Visualizer::Write(0, 18, L"||................................|       |                    |      |                    |       |");
        Visualizer::Write(0, 19, L"||................................|       |--------------------|      |--------------------|       |");
        Visualizer::Write(0, 20, L"||................................|                                       Item on ground           |");
        Visualizer::Write(0, 21, L"||--------------------------------|                                   |--------------------|       |");
        Visualizer::Write(0, 22, L"|                                                                     |                    |       |");
        Visualizer::Write(0, 23, L"|                                                                     |                    |       |");
        Visualizer::Write(0, 24, L"|                                                                     |                    |       |");
        Visualizer::Write(0, 25, L"|                                                                     |                    |       |");
        Visualizer::Write(0, 26, L"|                                                                     |                    |       |");
        Visualizer::Write(0, 27, L"|                                                                     |--------------------|       |");
        Visualizer::Write(0, 28, L"|                                                                                                  |");
        Visualizer::Write(0, 29, L"|                                                                                                  |");
        Visualizer::Write(0, 30, L"|                                                                                                  |");
        Visualizer::Write(0, 31, L"|                                                                                                  |");
        Visualizer::Write(0, 32, L"|                                                                                                  |");
        Visualizer::Write(0, 33, L"|                                                                                                  |");
        Visualizer::Write(0, 34, L"|                                                                                                  |");
        Visualizer::Write(0, 35, L"|                                                                                                  |");
        Visualizer::Write(0, 36, L"|                                                                                                  |");
        Visualizer::Write(0, 37, L"|                                                                                                  |");
        Visualizer::Write(0, 38, L"|                                                                                                  |");
        Visualizer::Write(0, 39, L"|                                                                                                  |");
        Visualizer::Write(0, 40, L"|                                                                                                  |");
        Visualizer::Write(0, 41, L"|                                                                                                  |");
        Visualizer::Write(0, 42, L"|                                                                                                  |");
        Visualizer::Write(0, 43, L"|                                                                                                  |");
        Visualizer::Write(0, 44, L"|                                                                                                  |");
        Visualizer::Write(0, 45, L"|                                                                                                  |");
        Visualizer::Write(0, 46, L"|                                                                                                  |");
        Visualizer::Write(0, 47, L"|                                                                                                  |");
        Visualizer::Write(0, 48, L"| actions: 'help <what>', 'move <direction>', 'cast <int or name>', 'prepare <name>', 'stablize'   |");
        Visualizer::Write(0, 49, L"|---------------------------------------------------------------------------------------------------");
    }

    public:
        void VisualDrawMap()
        {
            // tiels
            for (int y = 0; y < MY; y ++)
                for (int x = 0; x < MX; x ++)
                    if (InLOS(x, y))
                         Visualizer::Write(GetVisX(x), GetVisY(y), TILES.find(map[x + y * MX]).operator*().second);
                    else Visualizer::Write(GetVisX(x), GetVisY(y), TILES.find(-1).operator*().second);
            
            // enemies
            for (auto enemy : enemies)
                if (InLOS(enemy.x, enemy.y))
                    Visualizer::Write(GetVisX(enemy.x), GetVisY(enemy.y), enemy.visual);

            // player
            Visualizer::Write(GetVisX(player.x), GetVisY(player.y), player.visual);
        }

    private:
        int GetVisX(int x) { return 2 + GetMapX(x); }
        int GetVisY(int y) { return 5 + GetMapY(y); }
        int GetMapX(int x) { return (MX * 3/2 + x - player.x) % MX; }
        int GetMapY(int y) { return (MY * 3/2 + y - player.y) % MY; }

        bool InLOS(int tx, int ty) {

            // spell
            if (player.isSpellEyeActive) return true;

            int px = player.x, py = player.y;

            // fix fo recursive
            if (abs(tx - px + 1) > MX / 2)
            {
                if (tx < px) tx += MX;
                else         tx -= MX;
            }

            if (abs(ty - py + 1) > MY / 2)
            {
                if (ty < py) ty += MY;
                else         ty -= MY;
            }

            // find obstacles
            int len = std::max(abs(tx - px), abs(ty - py));
            for (int i = 0; i < len; i ++)
            {
                double step = double(i) / len;
                int x = (int)(MX + round(px * (1.0 - step) + tx * step)) % MX;
                int y = (int)(MY + round(py * (1.0 - step) + ty * step)) % MY;
                
                if (map[x + y * MX] == 1052688) return false;
            }

            // no obstacles found
            return true;
        }


    void VisualBackgroudIntro()
    {
        Visualizer::Write(0,  0, L"----------------------------------------------------------------------------------------------------");
        Visualizer::Write(0,  1, L"|                        ...but when you add dragon blood to quantum lexir,                        |");
        Visualizer::Write(0,  2, L"|                             you realize what was done. But to late.                              |");
        Visualizer::Write(0,  3, L"|                    There is not enough time for you to reach your spell book.                    |");
        Visualizer::Write(0,  4, L"|                   Fortunately, book is open on a page with five spells on it.                    |");
        Visualizer::Write(0,  5, L"|                         you managed to learn three spells most helpfull,                         |");
        Visualizer::Write(0,  6, L"|                         before universe distabilizes and you vaporize...                         |");
        Visualizer::Write(0,  7, L"|                                                                                                  |");
        Visualizer::Write(0,  8, L"|                                                                                                  |");
        Visualizer::Write(0,  9, L"|               |--------------------| |--------------------| |--------------------|               |");
        Visualizer::Write(0, 10, L"|               |                    | |                    | |                    |               |");
        Visualizer::Write(0, 11, L"|               |                    | |                    | |                    |               |");
        Visualizer::Write(0, 12, L"|               |                    | |                    | |                    |               |");
        Visualizer::Write(0, 13, L"|               |                    | |                    | |                    |               |");
        Visualizer::Write(0, 14, L"|               |                    | |                    | |                    |               |");
        Visualizer::Write(0, 15, L"|               |--------------------| |--------------------| |--------------------|               |");
        Visualizer::Write(0, 16, L"|                                                                                                  |");
        Visualizer::Write(0, 17, L"|                          |--------------------|  |--------------------|                          |");
        Visualizer::Write(0, 18, L"|                          |                    |  |                    |                          |");
        Visualizer::Write(0, 19, L"|                          |                    |  |                    |                          |");
        Visualizer::Write(0, 20, L"|                          |                    |  |                    |                          |");
        Visualizer::Write(0, 21, L"|                          |                    |  |                    |                          |");
        Visualizer::Write(0, 22, L"|                          |                    |  |                    |                          |");
        Visualizer::Write(0, 23, L"|                          |--------------------|  |--------------------|                          |");
        Visualizer::Write(0, 24, L"|                                                                                                  |");
        Visualizer::Write(0, 25, L"|                                                                                                  |");
        Visualizer::Write(0, 26, L"|                                                                                                  |");
        Visualizer::Write(0, 27, L"|                                                                                                  |");
        Visualizer::Write(0, 28, L"|                                                                                                  |");
        Visualizer::Write(0, 29, L"|                                                                                                  |");
        Visualizer::Write(0, 30, L"|                                                                                                  |");
        Visualizer::Write(0, 31, L"|                                                                                                  |");
        Visualizer::Write(0, 32, L"|                                                                                                  |");
        Visualizer::Write(0, 33, L"|                                                                                                  |");
        Visualizer::Write(0, 34, L"|                                                                                                  |");
        Visualizer::Write(0, 35, L"|                                                                                                  |");
        Visualizer::Write(0, 36, L"|                                                                                                  |");
        Visualizer::Write(0, 37, L"|                                                                                                  |");
        Visualizer::Write(0, 38, L"|                                                                                                  |");
        Visualizer::Write(0, 39, L"|                                                                                                  |");
        Visualizer::Write(0, 40, L"|                                                                                                  |");
        Visualizer::Write(0, 41, L"|                                                                                                  |");
        Visualizer::Write(0, 42, L"|                                                                                                  |");
        Visualizer::Write(0, 43, L"|                                                                                                  |");
        Visualizer::Write(0, 44, L"|                                                                                                  |");
        Visualizer::Write(0, 45, L"|                                                                                                  |");
        Visualizer::Write(0, 46, L"|                                                                                                  |");
        Visualizer::Write(0, 47, L"|                                                                                                  |");
        Visualizer::Write(0, 48, L"|                                                                                                  |");
        Visualizer::Write(0, 49, L"|---------------------------------------------------------------------------------------------------");
    }


    void VisaualSetSpell(int x, int y, int id)
    {

    }

    void VisaualBorderSpellDefault(int x, int y)
    {
        Visualizer::Write(x, y+0, L"┏━━━━━━━━━━━━━━━━━━━━┓");
        Visualizer::Write(x, y+1, L"┃~~~~~~~~~~~~~~~~~~~~┃");
        Visualizer::Write(x, y+2, L"┃~~~~~~~~~~~~~~~~~~~~┃");
        Visualizer::Write(x, y+3, L"┃~~~~~~~~~~~~~~~~~~~~┃");
        Visualizer::Write(x, y+4, L"┃~~~~~~~~~~~~~~~~~~~~┃");
        Visualizer::Write(x, y+5, L"┃~~~~~~~~~~~~~~~~~~~~┃");
        Visualizer::Write(x, y+6, L"┗━━━━━━━━━━━━━━━━━━━━┛");
    }
    void VisaualBorderSpellSelected(int x, int y)
    {
        Visualizer::Write(x, y+0, L"╔════════════════════╗");
        Visualizer::Write(x, y+1, L"║~~~~~~~~~~~~~~~~~~~~║");
        Visualizer::Write(x, y+2, L"║~~~~~~~~~~~~~~~~~~~~║");
        Visualizer::Write(x, y+3, L"║~~~~~~~~~~~~~~~~~~~~║");
        Visualizer::Write(x, y+4, L"║~~~~~~~~~~~~~~~~~~~~║");
        Visualizer::Write(x, y+5, L"║~~~~~~~~~~~~~~~~~~~~║");
        Visualizer::Write(x, y+6, L"╚════════════════════╝");
    }
    void VisaualBorderSpellDisabled(int x, int y)
    {
        Visualizer::Write(x, y+0, L"┌────────────────────┐");
        Visualizer::Write(x, y+1, L"│~~~~~~~~~~~~~~~~~~~~│");
        Visualizer::Write(x, y+2, L"│~~~~~~~~~~~~~~~~~~~~│");
        Visualizer::Write(x, y+3, L"│~~~~~~~~~~~~~~~~~~~~│");
        Visualizer::Write(x, y+4, L"│~~~~~~~~~~~~~~~~~~~~│");
        Visualizer::Write(x, y+5, L"│~~~~~~~~~~~~~~~~~~~~│");
        Visualizer::Write(x, y+6, L"└────────────────────┘");
    }

    public:
        void Run()
        {
            std::string task;


            while (true)
            {  
                // draw map
                VisualDrawMap();
                Visualizer::Out();

                // get task and args 
                getline(std::cin, task);

                // check command
                /// system
                if (task == "quit") break;

                /// movement
                else if (task == "mu" || task == "move up"   ) 
                {
                    int newPlayerY = (MY + player.y - 1) % MY;
                    if (IsNotObstacle(player.x, newPlayerY)) 
                        player.y = newPlayerY;
                }
                else if (task == "ml" || task == "move left" )
                {
                    int newPlayerX = (MX + player.x - 1) % MX;
                    if (IsNotObstacle(newPlayerX, player.y)) 
                        player.x = newPlayerX;
                }
                else if (task == "md" || task == "move down" )
                {
                    int newPlayerY = (MY + player.y + 1) % MY;
                    if (IsNotObstacle(player.x, newPlayerY)) 
                        player.y = newPlayerY;
                }
                else if (task == "mr" || task == "move right")
                {
                    int newPlayerX = (MX + player.x + 1) % MX;
                    if (IsNotObstacle(newPlayerX, player.y)) 
                        player.x = newPlayerX;
                }

                /// spells
                else if (task == "ce" || task == "cast eye"  )
                {
                    player.isSpellEyeActive = !player.isSpellEyeActive;
                }
                
                /// unknown command
                else continue;

                // enemy turn
            }
        }

    bool IsNotObstacle(int x, int y)
    {
        if (map[x + y * MX] == 1052688) return false;
        for (auto enemy : enemies) if (enemy.x == x && enemy.y == y) return false;
        return true;
    }

    void Intro()
    {
        VisualBackgroudIntro();
        // player = Player();
    }
};


const std::tuple<std::string, std::string> Game::MAPS[]
{
    {"The Entrance", "theentrance.png"},
    {"Hostile Cave", "hostilecave.png"},
};
const int Game::GENTRYLIMIT = 5;


const std::map<int, const std::wstring> Game::TILES
{
    {-1,       L"*"}, // DARK
    {1052688,  L"█"}, // WALL
    {10526880, L" "}, // FLOOR
    {1052832,  L" "}, // SPAWNER
    {1089552,  L" "}, // TRAP
};