#ifndef __ENGINE_ENGINE_H
#define __ENGINE_ENGINE_H



// +++ INCLUDES +++
#include "color.h"
#include <mutex>
#include <curses.h>



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

        /// @brief Renders text on window.
        /// @param __win window to render on.
        /// @param __x text begin position on the window on x axis.
        /// @param __y text begin position on the window on y axis.
        /// @param __text text to  be renderer on the window.
        /// @param __colorF font color of the text rendered.
        /// @param __colorB background color of the text rendered.
        // void RenderText(WINDOW* __win, int __x, int __y, const char* __text, Color __colorF, Color __colorB);
    };
};



#endif // __ENGINE_ENGINE_H