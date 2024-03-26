#pragma once

#include <tuple>

class Controls {
    public:
        static void Init();
    private:
        static bool initialized;

    public:
        static bool is_running;
        static void Run();
    private:
        static void HandleMouseMovement(int __x, int __y);
        static void HandleMouseButton1(int __x, int __y);
};