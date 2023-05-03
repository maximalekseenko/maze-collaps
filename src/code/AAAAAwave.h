#pragma once

#include <vector>
#include <cmath>
#include <tuple>
#include <random>
#include <functional>



class Model
{
    protected: 

        /// @brief wave[i][t]; Can tile(t) be at node(i).
        std::vector<std::vector<bool>> wave;

    protected: 

        /// @brief propagator[d][t][c] list of tiles(c), that can be in direction(d) to tile(t2)
        std::vector<std::vector<std::vector<int>>> propagator;

    protected: 
        std::vector<int> observed;


    private: // +++RUN UNIQUE MEMBERS+++


        /// @brief compatible[i][t][4]
        std::vector<std::vector<std::vector<int>>> compatible;

        /// @brief stack[<i, t>]; banned, but not propagated states.
        std::vector<std::tuple<int, int>> stack;
        
        /// @brief
        int observedSoFar;


    protected:


        /// @brief width of space to collapse
        int MX;

        /// @brief height of space to collapse
        int MY;

        int T;

        /// @brief width and height of pattern
        int N;


    protected: 
        bool periodic;
        bool ground;

    protected: std::vector<double> weights;
    private: std::vector<double> weightLogWeights, distribution;

    /// @brief sum of possible states by node
    protected: std::vector<int> sumsOfOnes;

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


    public: enum Heuristic { Entropy, MRV, Scanline };
    private: Heuristic heuristic;


    protected: // +++CONSTRUCTORS AND DESTRUCTORS+++
        
        Model(int width, int height, int N, bool periodic, Heuristic heuristic)
                : MX(width), MY(height), N(N), 
                    periodic(periodic), heuristic(heuristic)
        {}


    private: 
        void Init()
        {
            wave = std::vector<std::vector<bool>>(MX * MY);
            compatible = std::vector<std::vector<std::vector<int>>>(wave.size());
            for (int i = 0; i < wave.size(); i++)
            {
                wave[i] = std::vector<bool>(T);
                compatible[i] = std::vector<std::vector<int>>(T);
                for (int t = 0; t < T; t++) compatible[i][t] = std::vector<int>(4);
            }
            distribution = std::vector<double>(T);
            observed = std::vector<int>(MX * MY);

            weightLogWeights = std::vector<double>(T);
            sumOfWeights = 0;
            sumOfWeightLogWeights = 0;

            for (int t = 0; t < T; t++)
            {
                weightLogWeights[t] = weights[t] * std::log(weights[t]);
                sumOfWeights += weights[t];
                sumOfWeightLogWeights += weightLogWeights[t];
            }

            startingEntropy = std::log(sumOfWeights) - sumOfWeightLogWeights / sumOfWeights;

            sumsOfOnes = std::vector<int>(MX * MY);
            sumsOfWeights = std::vector<double>(MX * MY);
            sumsOfWeightLogWeights = std::vector<double>(MX * MY);
            entropies = std::vector<double>(MX * MY);

            stack = std::vector<std::tuple<int, int>>(wave.size() * T);
        }

    /// @brief run and generate
    public: bool Run(int seed, int limit)
    {
        if (wave.size() == 0) Init();

        Clear();

        std::random_device dev;
        std::mt19937 rng(seed < 0 ? dev() : seed);
        std::uniform_real_distribution<double> dist6(0, 1);
        auto random = [&dist6, &rng]{ return dist6(rng); };

        for (int l = 0; l < limit || limit < 0; l++)
        {
            int node = NextUnobservedNode(random);
            if (node >= 0)
            {
                Observe(node, random);
                bool success = Propagate();
                if (!success) return false;
            }
            else
            {
                for (int i = 0; i < wave.size(); i++) for (int t = 0; t < T; t++) if (wave[i][t]) { observed[i] = t; break; }
                return true;
            }
        }

        return true;
    }

    /// @brief Get next node to observe
    /// @param random
    /// @return 
    private: int NextUnobservedNode(std::function<double()> random)
    {
        if (heuristic == Heuristic::Scanline)
        {
            for (int i = observedSoFar; i < wave.size(); i++)
            {
                if (!periodic && (i % MX + N > MX || i / MX + N > MY)) continue;
                if (sumsOfOnes[i] > 1)
                {
                    observedSoFar = i + 1;
                    return i;
                }
            }
            return -1;
        }

        double min = 1E+4;
        int argmin = -1;
        for (int i = 0; i < wave.size(); i++)
        {
            if (!periodic && (i % MX + N > MX || i / MX + N > MY)) continue;
            int remainingValues = sumsOfOnes[i];
            double entropy = heuristic == Heuristic::Entropy ? entropies[i] : remainingValues;
            if (remainingValues > 1 && entropy <= min)
            {
                double noise = 1E-6 * random();
                if (entropy + noise < min)
                {
                    min = entropy + noise;
                    argmin = i;
                }
            }
        }
        return argmin;
    }

    /// @brief Observe node and collapse it
    /// @param node index of the node to collapse
    /// @param random 
    private: void Observe(int node, std::function<double()> random)
    {
        std::vector<bool>& w = wave[node];

        for (int t = 0; t < T; t++) 
            distribution[t] = w[t] ? weights[t] : 0.0;

        int r = distribution[(int)(random() * T)];

        for (int t = 0; t < T; t++) 
            if (w[t] != (t == r)) 
                Ban(node, t);
    }

    /// @brief 
    /// @return 
    private: bool Propagate()
    {
        while (stack.size() > 0)
        {
            auto [i1, t1] = stack.back();
            stack.pop_back();

            int x1 = i1 % MX;
            int y1 = i1 / MX;

            for (int d = 0; d < 4; d++)
            {
                int x2 = x1 + dx[d];
                int y2 = y1 + dy[d];
                if (!periodic && (x2 < 0 || y2 < 0 || x2 + N > MX || y2 + N > MY)) continue;

                if (x2 < 0) x2 += MX;
                else if (x2 >= MX) x2 -= MX;
                if (y2 < 0) y2 += MY;
                else if (y2 >= MY) y2 -= MY;

                int i2 = x2 + y2 * MX;
                std::vector<int> p = propagator[d][t1];
                std::vector<std::vector<int>> compat = compatible[i2];

                for (int l = 0; l < p.size(); l++)
                {
                    int t2 = p[l];
                    std::vector<int> comp = compat[t2];

                    comp[d]--;
                    if (comp[d] == 0) Ban(i2, t2);
                }
            }
        }

        return sumsOfOnes[0] > 0;
    }

    /// @brief bans pattern at node
    /// @param i index of the node to ban
    /// @param t index of the pattern to ban
    private: void Ban(int i, int t)
    {
        // remove pattern from node possibility
        wave[i][t] = false;

        std::vector<int> comp = compatible[i][t];
        for (int d = 0; d < 4; d++) comp[d] = 0;
        stack.push_back({i, t});

        sumsOfOnes[i] -= 1;
        sumsOfWeights[i] -= weights[t];
        sumsOfWeightLogWeights[i] -= weightLogWeights[t];

        double sum = sumsOfWeights[i];
        entropies[i] = std::log(sum) - sumsOfWeightLogWeights[i] / sum;
    }

    private: void Clear()
    {
        for (int i = 0; i < wave.size(); i++)
        {
            for (int t = 0; t < T; t++)
            {
                wave[i][t] = true;
                for (int d = 0; d < 4; d++) compatible[i][t][d] = propagator[opposite[d]][t].size();
            }

            sumsOfOnes[i] = weights.size();
            sumsOfWeights[i] = sumOfWeights;
            sumsOfWeightLogWeights[i] = sumOfWeightLogWeights;
            entropies[i] = startingEntropy;
            observed[i] = -1;
        }
        observedSoFar = 0;


        if (ground)
        {
            for (int x = 0; x < MX; x++)
            {
                for (int t = 0; t < T - 1; t++) Ban(x + (MY - 1) * MX, t);
                for (int y = 0; y < MY - 1; y++) Ban(x + y * MX, T - 1);
            }
            Propagate();
        }
    }

    public: void Save(std::string filename);

    protected: const static int dx[4];
    protected: const static int dy[4];
    private: const static int opposite[4];
};

const int Model::dx[4] = { -1, 0, 1, 0 };
const int Model::dy[4] = { 0, 1, 0, -1 };
const int Model::opposite[4] = { 2, 3, 0, 1 };