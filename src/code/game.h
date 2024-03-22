#pragma once



#include <vector>

#include "entity/entity.h"
#include "map.h"



struct Game
{
    public:
        /// @brief placeholder untill i find a better way to acces current game instance.
        static Game game;

    public: // +++ CONSTRUCTORS AND DESTRUCTORS +++ //
        Game();
        ~Game();

    public: // +++ ACCESSABLE RUNTIME MEMBERS +++ //
        Map *current_map;
        std::vector<Entity*> entities;

    public: // 

        /// @brief Begins circle of running the game.
        void Run();

        Entity* MakeEntity(int __pos);

    public: 
        /// @brief Loads map into the game and initialize it with Game::InitializeMap.
        /// @param __mapFileName name of mapFile to load 
        void LoadMap(std::string __mapFileName);

        /// @brief Initializes map by creating all entities. Calls Game::ClearMap on run.
        /// @param __map map to initialize
        void InitializeMap(Map *__map);

        /// @brief Clears all map related variables (e.g. entities)
        void ClearMap();
        

};