#pragma once


// #include "rendrule.h"



class Renderer {        
    public: // +++ INITIALIZATION +++ //
        static void Init();


    public:
        static void Run();


    public: // TEMP
        enum Color {
            BLACK     = 0, BRIGHT_BLACK     = 8,
            RED       = 1, BRIGHT_RED       = 9,
            GREEN     = 2, BRIGHT_GREEN     = 10,
            YELLOW    = 3, BRIGHT_YELLOW    = 11,
            BLUE      = 4, BRIGHT_BLUE      = 12,
            MAGENTA   = 5, BRIGHT_MAGENTA   = 13,
            CYAN      = 6, BRIGHT_CYAN      = 14,
            WHITE     = 7, BRIGHT_WHITE     = 15
        };
        static void RenderText(int __x, int __y, const char* __text, Color __colorF, Color __colorB); 
};