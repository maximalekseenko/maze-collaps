#pragma once


#include <random>



struct Random
{
    private: // +++MEMBERS+++
        static std::uniform_real_distribution<double> dist;
        static std::mt19937 gen;


    public: // +++FUNCTIONS+++
        static void Seed(int seed = -1)
        {
            gen = std::mt19937(seed == -1 ? std::random_device()() : seed);
        }

        static double Get()
        {
            return dist(gen);
        }
};


std::uniform_real_distribution<double> Random::dist = std::uniform_real_distribution<double>();
std::mt19937 Random::gen = std::mt19937(std::random_device()());