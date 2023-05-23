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
#include "log.h"



struct Game
{
    public: // +++LOAD MAP+++
        static void LoadMap(int id)
        {
            Map::Load(id);

            MapInit();

            UserInterface::UpdateAll();
        }

    
    private: // +++LOAD MAP+++

        static void MapInit()
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


    public: // +++RUN+++
        static void Run()
        {
            while (true)
            {  
                // player
                while (Player::player.Turn() == TURNCONTINUE) {}

                // enemies
                for (auto& enemy : Enemy::enemies)
                    enemy.Turn(Player::player.position);

                // loss
                for (auto& enemy : Enemy::enemies)
                    if (enemy.position == Player::player.position) 
                        throw std::runtime_error("ERROR: GAME LOST");
                        
                // update
                UserInterface::Update();

                // victory
                if (Enemy::enemies.size() == 0 && !Map::LoadNext()) throw std::runtime_error("ERROR: VICTORY");

            }
        }
};