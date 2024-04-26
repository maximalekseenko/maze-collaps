#ifndef __ENGINE_CURSES_H
#define __ENGINE_CURSES_H


#include <mutex>
#ifdef __MINGW32__
    #include <ncurses/curses.h>
#else
    #include <curses.h>
#endif


namespace Curses
{
    /// @brief Used to lock curses to solve curses' problems with threading.
    extern std::recursive_mutex curses_lock;
}


#endif // __ENGINE_CURSES_H