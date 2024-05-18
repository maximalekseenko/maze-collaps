#pragma once



#include <vector>

#include "entity/entity.h"
#include "map/map.h"



struct Game
{
    public: // +++ ACCESSABLE RUNTIME MEMBERS +++ //
        static Map *current_map;
        static std::vector<Entity*> entities;

    public: // 

        /// @brief Begins circle of running the game.
        static void Run();

        static Entity* MakeEntity(int __pos);

    public: 
        /// @brief Loads map into the game and initialize it with Game::InitializeMap.
        /// @param __mapFileName name of mapFile to load 
        static void LoadMap(std::string __mapFileName);

        /// @brief Initializes map by creating all entities. Calls Game::ClearMap on run.
        /// @param __map map to initialize
        static void InitializeMap(Map *__map);

        /// @brief Clears all map related variables (e.g. entities)
        static void ClearMap();
        

};