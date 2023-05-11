#pragma once


#include <iostream>
#include <stdlib.h>
#include <string>



struct Visualizer
{
    private:
        static int width;
        static int height;
        static std::wstring screen;

    private:
        static bool isChanged;
    
    public:

    static void Init(int w, int h)
    {
        width = w;
        height = h;
        // for (int i = 0; i < w * h; i ++) screen.append(u" ");
        screen = std::wstring(w * h, ' ');
    }

    static void Write(int x, int y, const std::wstring line)
    {

        // write
        for (int i = 0; i < line.size(); i ++) 
            if (line[i] != '~')
                screen[x + y * width + i] = line[i];

        // changed
        isChanged = true;
    }


    static void Clear()
    {
        for (int i = 0; i < width * height; i ++)
            screen[i] = ' ';
    }


    static void Out()
    {
        // if (!isChanged) return;

        // clear
        #if defined _WIN32
            std::wcout << "\x1B[2J\x1B[H";
        #elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
            std::wcout<< u8"\033[2J\033[1;1H"; 
        #elif defined (__APPLE__)
            std::wcout << "\33c\e[3J";
        #endif


        // out screen
        for (int y = 0; y < height; y ++)
        {
            for (int x = 0; x < width; x ++)
            {
                std::wcout << screen[x + y * width];
            }
            std::wcout << std::endl;
        }

        isChanged = false;
    }
};

int   Visualizer::width;
int   Visualizer::height;
std::wstring Visualizer::screen;
bool  Visualizer::isChanged;
