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

            UserInterface::UpdateAll();
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


    public: // +++RUN+++
        void Run()
        {
            while (true)
            {  
                // player
                while (Player::player.Turn() == TURNCONTINUE) {}

                // enemies
                for (auto& enemy : Enemy::enemies) {
                    enemy.Turn(Player::player.position);
                    if (enemy.position == Player::player.position) throw std::runtime_error("ERROR: GAME LOST");
                }

                // update
                UserInterface::Update();
            }
        }
};