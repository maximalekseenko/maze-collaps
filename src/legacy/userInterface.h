#pragma once

#include <ncurses.h>
#include "map.h"


struct UserInterface
{
    private: // +++WINDOWS+++
        static WINDOW* WGlobal;
        static WINDOW* WMap;
        static WINDOW* WSlot0;
        static WINDOW* WSlot1;
        static WINDOW* WSlot2;


    public: 
        static void Init();
        static void Exit();


    public: // +++Updates+++
        static void Update(Map* __map);
        static void UpdateAll(Map* __map);
        static void DrawBackground(Map* __map);
        static void DrawMap(Map* __map);
        static void DrawSlots(Map* __map);

    public: // +++INPUTS+++
        static int Input();
};
