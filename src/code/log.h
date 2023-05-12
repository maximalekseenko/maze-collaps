#pragma once

#include <string>
#include <fstream>


struct Log
{
    private:
        static std::ofstream outfile;
        static void Init();
    public:
        static void Out(std::string text);
};