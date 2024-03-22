#include "random.h"


#include <random>



// +++MEMBERS+++
std::uniform_real_distribution<double> Random::dist = std::uniform_real_distribution<double>();
std::mt19937 Random::gen = std::mt19937(std::random_device()());


// +++FUNCTIONS+++
void Random::Seed(int seed = -1)
{
    gen = std::mt19937(seed == -1 ? std::random_device()() : seed);
}

double Random::Get()
{
    return dist(gen);
}
