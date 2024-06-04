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
private: // core variables
    static Mapdata *mapdata;

    /// @brief wave[i][p]; Can pattern(p) be at node(i).
    static bool **wave;
    /// @brief compatible[i][p][d]; Can pattern(p) be in direction(d) from node(i).
    static int ***compatible;

private: // run-time support variables
    /// @brief Random distributor
    static std::uniform_real_distribution<double> dist;
    /// @brief stack[<i, p>]; banned, but not propagated patterns(p) at node(i).
    static std::vector<std::tuple<int, int>> stack;

private:
    /// @brief dfsdk
    static double *weightLogWeights;
    /// @brief Sum of all patterns' weights.
    static double sumOfWeights;
    /// @brief sum of log weigthsiodfsniosdndossnoinosidvonddsvoindsvion
    static double sumOfWeightLogWeights;
    /// @brief Starting entropy value.
    static double startingEntropy;

protected:
    /// @brief possiblePatternsLeft[i]; Amount of patterns possible on the node(i).
    static int *possiblePatternsLeft;
    /// @brief sumsOfWeights[i]; Sum of possible patterns' weights at node(i).
    static double *sumsOfWeights;
    /// @brief sumsOfWeightLogWeights[i]; Sum of lodfjsfobnsfonsodso
    static double *sumsOfWeightLogWeights;
    /// @brief entropies[i]; Entropy at node;
    static double *entropies;

public:
    static Map *Generate(Mapdata *__mapdata, int __seed = -1, int __attemptsLimit = -1, int __stepsLimit = -1);

private:
    static void Init();
    static void Clear();
    static void Deinit();

    /// @brief Collapse wave
    /// @param __seed Seed for random generator;
    /// @param __stepsLimit Limit of steps
    /// @return
    static bool Run(int __seed, int __stepsLimit);

    static Map *Save();

    /// @brief Get next node to observe
    static int NextUnobservedNode(std::mt19937 &gen);

    /// @brief Observe node and collapse it
    /// @param node index of the node to collapse
    static void Observe(int node, std::mt19937 &gen);

    /// @brief
    /// @return
    static bool Propagate();

    /// @brief bans pattern at node
    /// @param i index of the node to ban
    /// @param t index of the pattern to ban
    static void Ban(int i, int t);
};

#endif // __MAP_GENERATOR_H