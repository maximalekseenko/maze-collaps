#pragma once

#include <ncurses.h>


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
        static void Update();
        static void UpdateAll();
        static void DrawBackground();
        static void DrawMap();
        static void DrawSlots();

    public: // +++INPUTS+++
        static char Input();
};
