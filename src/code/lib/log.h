#pragma once

#include <string>
#include <fstream>
#include <stdexcept>


/// @brief Utility that Makes outputs.
namespace Log
{
    /// @brief Make output to the oufile
    /// @param text 
    /// @param is_error 
    static void Out(std::string text, bool is_error=false) {
        std::ofstream outfile;
        outfile.open("log.txt", std::ios_base::app);
        outfile << text << std::endl;

        // if error: throw error
        if (is_error) throw text;
    }
};