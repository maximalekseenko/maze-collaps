#pragma once

#include <string>
#include <fstream>


struct Log
{
    public:
        static void Out(std::string text) {
            std::ofstream outfile;
            outfile.open("log.txt", std::ios_base::app);
            outfile << text << std::endl;
        }
};