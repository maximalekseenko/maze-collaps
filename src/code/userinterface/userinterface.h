#pragma once

#include <mutex>

class UserInterface
{
    public:
        static std::recursive_mutex ncurses_lock;
    public:
        // static void Init();
        // static void Run();
};