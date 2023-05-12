#include "log.h"


void Log::Init()
{
    // stream.open("log.txt", std::ios_base::app);
}
void Log::Out(std::string text)
{
    // if (!stream.is_open()) Init();
    // stream << text;

    std::ofstream outfile;
    outfile.open("log.txt", std::ios_base::app);
    outfile << text << std::endl;
}

std::ofstream Log::outfile = std::ofstream();