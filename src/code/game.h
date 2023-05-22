#pragma once



#include <tuple>
#include <iostream>
#include <string>
#include <map>
#include <vector>


#include "player.h"
#include "enemy.h"
#include "map.h"

#include "userInterface.h"




struct Game
{


    public: // +++LOAD MAP+++
        void LoadMap(int id)
        {
            
            Map::Load(id);

            MapInit();
        }

    
    private: // +++LOAD MAP+++

        void MapInit()
        {

            Enemy::enemies.clear();
            bool isPlayerCreated = false;
            for (int i = 0; i < Map::MI; i ++)
            {
                // if spawner
                if (Map::Get(i) == Map::TILE::SPAWNER)
                {
                    // spawn player
                    if (!isPlayerCreated) 
                    {
                        isPlayerCreated = true;
                        Player::player = Player(i);
                    }
                    // spawn enemy
                    else
                    {
                        Enemy::Create(i);
                    }
                }
            }
        }


    // void VisualBackgroudMap()
    // {
    //     Visualizer::Write(0,  0, L"----------------------------------------------------------------------------------------------------");
    //     Visualizer::Write(0,  1, L"|                                                                                                  |");
    //     Visualizer::Write(0,  2, L"|                                                                                                  |");
    //     Visualizer::Write(0,  3, L"|                                                                                                  |");
    //     Visualizer::Write(0,  4, L"||--------------------------------|              Spell 1                     Spell 2               |");
    //     Visualizer::Write(0,  5, L"||................................|       |--------------------|      |--------------------|       |");
    //     Visualizer::Write(0,  6, L"||................................|       |                    |      |                    |       |");
    //     Visualizer::Write(0,  7, L"||................................|       |                    |      |                    |       |");
    //     Visualizer::Write(0,  8, L"||................................|       |                    |      |                    |       |");
    //     Visualizer::Write(0,  9, L"||................................|       |                    |      |                    |       |");
    //     Visualizer::Write(0, 10, L"||................................|       |                    |      |                    |       |");
    //     Visualizer::Write(0, 11, L"||................................|       |--------------------|      |--------------------|       |");
    //     Visualizer::Write(0, 12, L"||................................|              Spell 3                       Item                |");
    //     Visualizer::Write(0, 13, L"||................................|       |--------------------|      |--------------------|       |");
    //     Visualizer::Write(0, 14, L"||................................|       |                    |      |                    |       |");
    //     Visualizer::Write(0, 15, L"||................................|       |                    |      |                    |       |");
    //     Visualizer::Write(0, 16, L"||................................|       |                    |      |                    |       |");
    //     Visualizer::Write(0, 17, L"||................................|       |                    |      |                    |       |");
    //     Visualizer::Write(0, 18, L"||................................|       |                    |      |                    |       |");
    //     Visualizer::Write(0, 19, L"||................................|       |--------------------|      |--------------------|       |");
    //     Visualizer::Write(0, 20, L"||................................|                                       Item on ground           |");
    //     Visualizer::Write(0, 21, L"||--------------------------------|                                   |--------------------|       |");
    //     Visualizer::Write(0, 22, L"|                                                                     |                    |       |");
    //     Visualizer::Write(0, 23, L"|                                                                     |                    |       |");
    //     Visualizer::Write(0, 24, L"|                                                                     |                    |       |");
    //     Visualizer::Write(0, 25, L"|                                                                     |                    |       |");
    //     Visualizer::Write(0, 26, L"|                                                                     |                    |       |");
    //     Visualizer::Write(0, 27, L"|                                                                     |--------------------|       |");
    //     Visualizer::Write(0, 28, L"|                                                                                                  |");
    //     Visualizer::Write(0, 29, L"|                                                                                                  |");
    //     Visualizer::Write(0, 30, L"|                                                                                                  |");
    //     Visualizer::Write(0, 31, L"|                                                                                                  |");
    //     Visualizer::Write(0, 32, L"|                                                                                                  |");
    //     Visualizer::Write(0, 33, L"|                                                                                                  |");
    //     Visualizer::Write(0, 34, L"|                                                                                                  |");
    //     Visualizer::Write(0, 35, L"|                                                                                                  |");
    //     Visualizer::Write(0, 36, L"|                                                                                                  |");
    //     Visualizer::Write(0, 37, L"|                                                                                                  |");
    //     Visualizer::Write(0, 38, L"|                                                                                                  |");
    //     Visualizer::Write(0, 39, L"|                                                                                                  |");
    //     Visualizer::Write(0, 40, L"|                                                                                                  |");
    //     Visualizer::Write(0, 41, L"|                                                                                                  |");
    //     Visualizer::Write(0, 42, L"|                                                                                                  |");
    //     Visualizer::Write(0, 43, L"|                                                                                                  |");
    //     Visualizer::Write(0, 44, L"|                                                                                                  |");
    //     Visualizer::Write(0, 45, L"|                                                                                                  |");
    //     Visualizer::Write(0, 46, L"|                                                                                                  |");
    //     Visualizer::Write(0, 47, L"|                                                                                                  |");
    //     Visualizer::Write(0, 48, L"| actions: 'help <what>', 'move <direction>', 'cast <int or name>', 'prepare <name>', 'stablize'   |");
    //     Visualizer::Write(0, 49, L"|---------------------------------------------------------------------------------------------------");
    // }

    public:
        // void VisualDrawMap()
        // {
        //     // AAA;
        // }

    // private:
    //     int GetVisX(int x) { return 2 + GetMapX(x); }
    //     int GetVisY(int y) { return 5 + GetMapY(y); }
    //     int GetMapX(int x) { return (Map::MX * 3/2 + x - Map::X(player.position)) % Map::MX; }
    //     int GetMapY(int y) { return (Map::MY * 3/2 + y - Map::Y(player.position)) % Map::MY; }




    // void VisualBackgroudIntro()
    // {
    //     Visualizer::Write(0,  0, L"----------------------------------------------------------------------------------------------------");
    //     Visualizer::Write(0,  1, L"|                        ...but when you add dragon blood to quantum lexir,                        |");
    //     Visualizer::Write(0,  2, L"|                             you realize what was done. But to late.                              |");
    //     Visualizer::Write(0,  3, L"|                    There is not enough time for you to reach your spell book.                    |");
    //     Visualizer::Write(0,  4, L"|                   Fortunately, book is open on a page with five spells on it.                    |");
    //     Visualizer::Write(0,  5, L"|                         you managed to learn three spells most helpfull,                         |");
    //     Visualizer::Write(0,  6, L"|                         before universe distabilizes and you vaporize...                         |");
    //     Visualizer::Write(0,  7, L"|                                                                                                  |");
    //     Visualizer::Write(0,  8, L"|                                                                                                  |");
    //     Visualizer::Write(0,  9, L"|               |--------------------| |--------------------| |--------------------|               |");
    //     Visualizer::Write(0, 10, L"|               |                    | |                    | |                    |               |");
    //     Visualizer::Write(0, 11, L"|               |                    | |                    | |                    |               |");
    //     Visualizer::Write(0, 12, L"|               |                    | |                    | |                    |               |");
    //     Visualizer::Write(0, 13, L"|               |                    | |                    | |                    |               |");
    //     Visualizer::Write(0, 14, L"|               |                    | |                    | |                    |               |");
    //     Visualizer::Write(0, 15, L"|               |--------------------| |--------------------| |--------------------|               |");
    //     Visualizer::Write(0, 16, L"|                                                                                                  |");
    //     Visualizer::Write(0, 17, L"|                          |--------------------|  |--------------------|                          |");
    //     Visualizer::Write(0, 18, L"|                          |                    |  |                    |                          |");
    //     Visualizer::Write(0, 19, L"|                          |                    |  |                    |                          |");
    //     Visualizer::Write(0, 20, L"|                          |                    |  |                    |                          |");
    //     Visualizer::Write(0, 21, L"|                          |                    |  |                    |                          |");
    //     Visualizer::Write(0, 22, L"|                          |                    |  |                    |                          |");
    //     Visualizer::Write(0, 23, L"|                          |--------------------|  |--------------------|                          |");
    //     Visualizer::Write(0, 24, L"|                                                                                                  |");
    //     Visualizer::Write(0, 25, L"|                                                                                                  |");
    //     Visualizer::Write(0, 26, L"|                                                                                                  |");
    //     Visualizer::Write(0, 27, L"|                                                                                                  |");
    //     Visualizer::Write(0, 28, L"|                                                                                                  |");
    //     Visualizer::Write(0, 29, L"|                                                                                                  |");
    //     Visualizer::Write(0, 30, L"|                                                                                                  |");
    //     Visualizer::Write(0, 31, L"|                                                                                                  |");
    //     Visualizer::Write(0, 32, L"|                                                                                                  |");
    //     Visualizer::Write(0, 33, L"|                                                                                                  |");
    //     Visualizer::Write(0, 34, L"|                                                                                                  |");
    //     Visualizer::Write(0, 35, L"|                                                                                                  |");
    //     Visualizer::Write(0, 36, L"|                                                                                                  |");
    //     Visualizer::Write(0, 37, L"|                                                                                                  |");
    //     Visualizer::Write(0, 38, L"|                                                                                                  |");
    //     Visualizer::Write(0, 39, L"|                                                                                                  |");
    //     Visualizer::Write(0, 40, L"|                                                                                                  |");
    //     Visualizer::Write(0, 41, L"|                                                                                                  |");
    //     Visualizer::Write(0, 42, L"|                                                                                                  |");
    //     Visualizer::Write(0, 43, L"|                                                                                                  |");
    //     Visualizer::Write(0, 44, L"|                                                                                                  |");
    //     Visualizer::Write(0, 45, L"|                                                                                                  |");
    //     Visualizer::Write(0, 46, L"|                                                                                                  |");
    //     Visualizer::Write(0, 47, L"|                                                                                                  |");
    //     Visualizer::Write(0, 48, L"|                                                                                                  |");
    //     Visualizer::Write(0, 49, L"|---------------------------------------------------------------------------------------------------");
    // }


    // void VisaualSetSpell(int x, int y, int id)
    // {
    // }
    // void VisaualBorderSpellDefault(int x, int y)
    // {
    //     Visualizer::Write(x, y+0, L"┏━━━━━━━━━━━━━━━━━━━━┓");
    //     Visualizer::Write(x, y+1, L"┃~~~~~~~~~~~~~~~~~~~~┃");
    //     Visualizer::Write(x, y+2, L"┃~~~~~~~~~~~~~~~~~~~~┃");
    //     Visualizer::Write(x, y+3, L"┃~~~~~~~~~~~~~~~~~~~~┃");
    //     Visualizer::Write(x, y+4, L"┃~~~~~~~~~~~~~~~~~~~~┃");
    //     Visualizer::Write(x, y+5, L"┃~~~~~~~~~~~~~~~~~~~~┃");
    //     Visualizer::Write(x, y+6, L"┗━━━━━━━━━━━━━━━━━━━━┛");
    // }
    // void VisaualBorderSpellSelected(int x, int y)
    // {
    //     Visualizer::Write(x, y+0, L"╔════════════════════╗");
    //     Visualizer::Write(x, y+1, L"║~~~~~~~~~~~~~~~~~~~~║");
    //     Visualizer::Write(x, y+2, L"║~~~~~~~~~~~~~~~~~~~~║");
    //     Visualizer::Write(x, y+3, L"║~~~~~~~~~~~~~~~~~~~~║");
    //     Visualizer::Write(x, y+4, L"║~~~~~~~~~~~~~~~~~~~~║");
    //     Visualizer::Write(x, y+5, L"║~~~~~~~~~~~~~~~~~~~~║");
    //     Visualizer::Write(x, y+6, L"╚════════════════════╝");
    // }
    // void VisaualBorderSpellDisabled(int x, int y)
    // {
    //     Visualizer::Write(x, y+0, L"┌────────────────────┐");
    //     Visualizer::Write(x, y+1, L"│~~~~~~~~~~~~~~~~~~~~│");
    //     Visualizer::Write(x, y+2, L"│~~~~~~~~~~~~~~~~~~~~│");
    //     Visualizer::Write(x, y+3, L"│~~~~~~~~~~~~~~~~~~~~│");
    //     Visualizer::Write(x, y+4, L"│~~~~~~~~~~~~~~~~~~~~│");
    //     Visualizer::Write(x, y+5, L"│~~~~~~~~~~~~~~~~~~~~│");
    //     Visualizer::Write(x, y+6, L"└────────────────────┘");
    // }

    public:
        void Run()
        {
            std::string task;


            while (true)
            {  
                // get task and args 
                // getline(std::cin, task);

                // // check command
                // /// system
                // if (task == "quit") break;

                // /// movement
                // else if (task == "mu" || task == "move up"   ) 
                // {
                //     if (!Map::IsNotObstacle(Map::Up(player.position))) continue;
                //     Map::Up(&player.position);
                // }
                // else if (task == "ml" || task == "move left" )
                // {
                //     if (!Map::IsNotObstacle(Map::Left(player.position))) continue;
                //     Map::Left(&player.position);
                // }
                // else if (task == "md" || task == "move down" )
                // {
                //     if (!Map::IsNotObstacle(Map::Down(player.position))) continue;
                //     Map::Down(&player.position);
                // }
                // else if (task == "mr" || task == "move right")
                // {
                //     if (!Map::IsNotObstacle(Map::Right(player.position))) continue;
                //     Map::Right(&player.position);
                // }

                // /// prep
                // else if (task == "pc" || task == "prepair chaos")
                //     player.Prep(Player::SpellSource::CHAOS);
                // else if (task == "pw" || task == "prepair wrath")
                //     player.Prep(Player::SpellSource::WRATH);
                // else if (task == "pd" || task == "prepair divinity")
                //     player.Prep(Player::SpellSource::DIVINITY);

                // /// cast
                // else if (task == "c" || task == "cast")
                //     player.Cast();

                // /// unknown command
                // else continue;

                // // enemy turn
                // for (auto& enemy : Enemy::enemies) {
                //     enemy.Turn(player.position);
                //     if (enemy.position == player.position) throw std::runtime_error("ERROR: GAME LOST");
                // }

                // // draw map
                // VisualDrawMap();
                // Visualizer::Out();
                UserInterface::Update();

                break;
            }
        }
};