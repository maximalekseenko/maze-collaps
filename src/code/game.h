#pragma once



#include <tuple>
#include <iostream>
#include <string>
#include <map>

#include "overlappingwave.h"
#include "player.h"

#include "visualizer.h"



struct Game
{
    public:
        int* map;
        int MX = 32, MY = 16;
        std::string mapName;
        int mapId;


    std::string command, arg1, arg2;


    void LoadMap(int mapId, int trys=-1)
    {
    //     this->mapId = mapId;

    //     auto generator = OverlappingWave(std::get<1>(MAPS[mapId]), 4, MX, MY, true, true, 8, false, Model::Heuristic::Entropy);
    //     for(; trys != 0; trys --) if (generator.Run(-1, -1)) break;
    //     if (trys == 0) throw std::runtime_error("LoadLevel trys limit reached");

    //     this->map = generator.Result();
    }


    void PrintMap()
    {
        Visualizer::Write(0,  0, L"----------------------------------------------------------------------------------------------------");
        Visualizer::Write(0,  1, L"|                                                                                                  |");
        Visualizer::Write(0,  2, L"|                                                                                                  |");
        Visualizer::Write(0,  3, L"|                                                                                                  |");
        Visualizer::Write(0,  4, L"|--------------------------------||                          Your spells                           |");
        Visualizer::Write(0,  5, L"|................................|||--------------------|--------------------|--------------------||");
        Visualizer::Write(0,  6, L"|................................|||                    |                    |                    ||");
        Visualizer::Write(0,  7, L"|................................|||                    |                    |                    ||");
        Visualizer::Write(0,  8, L"|................................|||                    |                    |                    ||");
        Visualizer::Write(0,  9, L"|................................|||                    |                    |                    ||");
        Visualizer::Write(0, 10, L"|................................|||                    |                    |                    ||");
        Visualizer::Write(0, 11, L"|................................|||--------------------|--------------------|--------------------||");
        Visualizer::Write(0, 12, L"|................................|||          Your item                    Item on ground         ||");
        Visualizer::Write(0, 13, L"|................................|||------------------------------|-------------------------------||");
        Visualizer::Write(0, 14, L"|................................|||                              |                               ||");
        Visualizer::Write(0, 15, L"|................................|||                              |                               ||");
        Visualizer::Write(0, 16, L"|................................|||                              |                               ||");
        Visualizer::Write(0, 17, L"|................................|||                              |                               ||");
        Visualizer::Write(0, 18, L"|................................|||                              |                               ||");
        Visualizer::Write(0, 19, L"|--------------------------------|||------------------------------|-------------------------------||");
        Visualizer::Write(0, 20, L"|                                                                                                  |");
        Visualizer::Write(0, 21, L"|                                                                                                  |");
        Visualizer::Write(0, 22, L"|                                                                                                  |");
        Visualizer::Write(0, 23, L"|                                                                                                  |");
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
        Visualizer::Write(x, y+1, L"┃                    ┃");
        Visualizer::Write(x, y+2, L"┃                    ┃");
        Visualizer::Write(x, y+3, L"┃                    ┃");
        Visualizer::Write(x, y+4, L"┃                    ┃");
        Visualizer::Write(x, y+5, L"┃                    ┃");
        Visualizer::Write(x, y+6, L"┗━━━━━━━━━━━━━━━━━━━━┛");
    }
    void VisaualBorderSpellSelected(int x, int y)
    {
        Visualizer::Write(x, y+0, L"╔════════════════════╗");
        Visualizer::Write(x, y+1, L"║                    ║");
        Visualizer::Write(x, y+2, L"║                    ║");
        Visualizer::Write(x, y+3, L"║                    ║");
        Visualizer::Write(x, y+4, L"║                    ║");
        Visualizer::Write(x, y+5, L"║                    ║");
        Visualizer::Write(x, y+6, L"╚════════════════════╝");
    }
    void VisaualBorderSpellDisabled(int x, int y)
    {
        Visualizer::Write(x, y+0, L"┌────────────────────┐");
        Visualizer::Write(x, y+1, L"│                    │");
        Visualizer::Write(x, y+2, L"│                    │");
        Visualizer::Write(x, y+3, L"│                    │");
        Visualizer::Write(x, y+4, L"│                    │");
        Visualizer::Write(x, y+5, L"│                    │");
        Visualizer::Write(x, y+6, L"└────────────────────┘");
    }


    void Run()
    {
        Intro();
    }

    void Intro()
    {
        VisualBackgroudIntro();
        
    }
};


const std::tuple<std::string, std::string> MAPS[]
{
    {"Hostile Cave", "hostilecave.png"},
    {"The Entrance", "theentrance.png"},
};


const std::map<int, std::string> TILES
{
    {-1,       "*"},// DARK
    {1052688,  "█"},// WALL
    {10526880, " "},// FLOOR
    {1052832,  " "},// SPAWNER
    {1089552,  " "},// TRAP
};