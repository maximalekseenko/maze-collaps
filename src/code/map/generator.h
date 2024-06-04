#ifndef __MAP_GENERATOR_H
#define __MAP_GENERATOR_H

#include "./mapdata.h"
#include "./direction.h"
#include "./map.h"
#include "./reader.h"
#include <random>
#include <vector>

// class Generator
// {
//     /// @brief Generates a map based on parameters provided.
//     /// @param __mapdata Mapdata with settings for generation.
//     /// @return Map filled with tiles.
//     Map *GenerateMap(Mapdata *__mapdata);
// };

class Generator
{
protected:
    /// @brief wave[i][p]; Can pattern(p) be at node(i).
    bool **wave = nullptr;

    Mapdata *mapdata;

private: // +++RUN UNIQUE MEMBERS+++
    /// @brief Random distributor
    std::uniform_real_distribution<double> dist;

    /// @brief compatible[i][t][4]
    int ***compatible;

    /// @brief stack[<i, t>]; banned, but not propagated states.
    std::vector<std::tuple<int, int>> stack;

protected:
    /// @brief weights[t]; weight of each unique value
    std::vector<double> weights;

private:
    std::vector<double> weightLogWeights, distribution;

    /// @brief sum of possible states by node
protected:
    std::vector<int> sumsOfOnes;

private:
    /// @brief sum of weights
    double sumOfWeights;
    /// @brief sum of log weigths
    double sumOfWeightLogWeights;
    /// @brief starting amount of states
    double startingEntropy;

protected:
    /// @brief sum of weigths by node
    std::vector<double> sumsOfWeights;
    /// @brief sum of log weigths by node
    std::vector<double> sumsOfWeightLogWeights;
    /// @brief current amount of states by node
    std::vector<double> entropies;

public: // +++CONSTRUCTORS AND DESTRUCTORS+++
    Generator(Mapdata *__mapdata);
    ~Generator();

public:
    static Map *Generate(Mapdata *__mapdata);
    /// @brief run and generate
    bool Run(int seed, int limit);

    void Clear();

    void Save(Map *&__map);

private:
    void Init();

    /// @brief Get next node to observe
    int NextUnobservedNode(std::mt19937 &gen);

    /// @brief Observe node and collapse it
    /// @param node index of the node to collapse
    void Observe(int node, std::mt19937 &gen);

    /// @brief
    /// @return
    bool Propagate();

    /// @brief bans pattern at node
    /// @param i index of the node to ban
    /// @param t index of the pattern to ban
    void Ban(int i, int t);
};

#endif // __MAP_GENERATOR_H