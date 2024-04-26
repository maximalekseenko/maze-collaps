#ifndef __ENGINE_CURSES_H
#define __ENGINE_CURSES_H


#include <mutex>
#ifdef __MINGW32__
    #include <ncursesw/ncurses.h>
#else
    #include <ncurses.h>
#endif


namespace Curses
{
    /// @brief Used to lock curses to solve curses' problems with threading.
    extern std::recursive_mutex curses_lock;
}


#endif // __ENGINE_CURSES_H