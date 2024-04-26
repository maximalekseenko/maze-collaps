#ifndef __ENGINE_CURSES_H
#define __ENGINE_CURSES_H

#ifdef __MINGW32__
#include <ncursesw/ncurses.h>
#else
#include <ncurses.h>
#endif

#include <mutex>

namespace Curses
{
    /// @brief Used to lock curses to solve curses' problems with threading.
    static std::recursive_mutex curses_lock;
}

#endif // __ENGINE_CURSES_H