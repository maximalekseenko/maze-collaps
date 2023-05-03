#pragma once

#include "AAAAAwave.h"
#include "image.h"
#include <functional>
#include <map>


class OverlappingModel : Model {


    private: // +++OVERLAPMODEL UNIQUE MEMBERS+++


        std::vector<std::vector<int>> patterns;
        std::vector<int> colors;


    private: // +++SUPPORT TILEWORK FUNCTIONS+++

        /// @brief Does pattern1 can be near pattern2 in direction
        static bool agrees(std::vector<int> pattern1, std::vector<int> pattern2, int dx, int dy, int N)
        {
            int xmin = dx < 0 ?    0   : dx, 
                xmax = dx < 0 ? dx + N : N, 
                ymin = dy < 0 ?    0   : dy, 
                ymax = dy < 0 ? dy + N : N;
            for (int y = ymin; y < ymax; y++) 
                for (int x = xmin; x < xmax; x++) 
                    if (pattern1[x + N * y] != pattern2[x - dx + N * (y - dy)]) 
                        return false;
            return true;
        }

        /// @brief 
        static std::vector<int> pattern(std::function<int(int, int)> f, int N)
        {
            std::vector<int> result(N * N);
            for (int y = 0; y < N; y++) for (int x = 0; x < N; x++) result[x + y * N] = f(x, y);
            return result;
        }

        /// @brief 
        static std::vector<int> rotate(std::vector<int> p, int N) 
        {
            return pattern([&p, &N](int x, int y){ return p[N - 1 - y + x * N]; }, N);
        }

        /// @brief 
        static std::vector<int> reflect(std::vector<int> p, int N)
        {
            return pattern([&p, &N](int x, int y){ return p[N - 1 - x + y * N]; }, N);
        }

        /// @brief 
        static long hash(std::vector<int> p, int C)
        {
            long result = 0, power = 1;
            for (int i = 0; i < p.size(); i++)
            {
                result += p[p.size() - 1 - i] * power;
                power *= C;
            }
            return result;
        };

    public: // +++CONSTRUCTORS AND DESTRUCTORS+++


        OverlappingModel(std::string name, int N, int width, int height, bool periodicInput, bool periodic, int symmetry, bool ground, Heuristic heuristic)
            : Model(width, height, N, periodic, heuristic)
        {
            int SX, SY;
            int* bitmap = Image::ParseImage(name.c_str(), SX, SY);
            std::vector<int> sample(SX, SY);
            colors = std::vector<int>();
            for (int i = 0; i < SX * SY; i++)
            {
                int color = bitmap[i];
                int k = 0;
                for (; k < colors.size(); k++) if (colors[k] == color) break;
                if (k == colors.size()) colors.push_back(color);
                sample[i] = k;
            }
            delete[] bitmap;

            patterns.clear();
            std::map<long, int> patternIndices;

            int C = colors.size();
            int xmax = periodicInput ? SX : SX - N + 1;
            int ymax = periodicInput ? SY : SY - N + 1;
            for (int y = 0; y < ymax; y++) for (int x = 0; x < xmax; x++)
                {
                    std::vector<int> ps[8];

                    ps[0] = pattern([&sample, &SX, &SY, &x, &y](int dx, int dy){ return sample[(x + dx) % SX + (y + dy) % SY * SX]; }, N);
                    ps[1] = reflect(ps[0], N);
                    ps[2] = rotate(ps[0], N);
                    ps[3] = reflect(ps[2], N);
                    ps[4] = rotate(ps[2], N);
                    ps[5] = reflect(ps[4], N);
                    ps[6] = rotate(ps[4], N);
                    ps[7] = reflect(ps[6], N);

                    for (int k = 0; k < symmetry; k++)
                    {
                        std::vector<int> p = ps[k];
                        long h = hash(p, C);
                        if (auto iterIndex = patternIndices.find(h); iterIndex != patternIndices.end())
                            weights[iterIndex->second] = weights[iterIndex->second] + 1;
                        else
                        {
                            patternIndices.insert({h, weights.size()});
                            weights.push_back(1.0);
                            patterns.push_back(p);
                        }
                    }
                }

            T = weights.size();
            this->ground = ground;

            propagator = std::vector<std::vector<std::vector<int>>>();
            for (int d = 0; d < 4; d++)
            {
                propagator[d] = std::vector<std::vector<int>>(T);
                for (int t = 0; t < T; t++)
                {
                    std::vector<int> list;
                    for (int t2 = 0; t2 < T; t2++) 
                        if (agrees(patterns[t], patterns[t2], dx[d], dy[d], N)) 
                            list.push_back(t2);
                    propagator[d][t] = std::vector<int>(list.size());
                    for (int c = 0; c < list.size(); c++) propagator[d][t][c] = list[c];
                }
            }
        }

    public: void Save(std::string filename)
    {
    }
};