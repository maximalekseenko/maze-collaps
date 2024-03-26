#pragma once

#include <tuple>

class Controls {
    public:
        static void Init();
    private:
        static bool initialized;

    public:
        void Run();
};