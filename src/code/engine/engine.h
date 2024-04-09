#ifndef __ENGINE_ENGINE_H
#define __ENGINE_ENGINE_H



// +++ INCLUDES +++
#include "color.h"
#include <mutex>



/// @brief Core of the program.
namespace Engine
{
    /// @brief Used to lock ncurses to solve ncurses' problems with threading.
    extern std::recursive_mutex ncurses_lock;

    /// @brief Initializes Renderer and Controller.
    void Init();

    /// @brief Starts Renderer and Controller.
    void Run();


    /// @brief Module of the engine, that handles user input.
    namespace Controller {

        /// @brief Initializes Controller.
        void Init();

        /// @brief Starts the Controller's loop.
        void Run();
    };


    /// @brief Module the of engine, that handles output for user.
    namespace Renderer {      
        
        /// @brief Initializes Renderer.  
        void Init();

        /// @brief Starts the Renderer's loop.
        void Run();

        /// @brief Sets background color of main window.
        /// @param __colorB New color for background.
        void SetBackground(Color __colorB);

        /// @brief Gets background color of main window.
        /// @return Background color of main window.
        Color GetBackground();
    };
};



#endif // __ENGINE_ENGINE_H