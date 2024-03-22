#pragma once

#include <random>


struct Random
{
    private: // +++MEMBERS+++
        static std::uniform_real_distribution<double> dist;
        static std::mt19937 gen;


    public: // +++FUNCTIONS+++
        static void Seed(int seed);
        static double Get();
};
