#pragma once

#include "wave.h"
#include "image.h"
#include <functional>
#include <map>


class OverlappingModel : public Model {


    private: // +++OVERLAPMODEL UNIQUE MEMBERS+++


        std::vector<int*> patterns;
        std::vector<int> colors;


    private: // +++SUPPORT TILEWORK FUNCTIONS+++

        /// @brief Does pattern1 can be near pattern2 in direction
        static bool agrees(int* pattern1, int* pattern2, int dx, int dy, int N)
        {
            int xmin = dx < 0 ?    0   : dx, 
                xmax = dx < 0 ? dx + N : N, 
                ymin = dy < 0 ?    0   : dy, 
                ymax = dy < 0 ? dy + N : N;
            for (int y = ymin; y < ymax; y++) 
                for (int x = xmin; x < xmax; x++) 
                    if (pattern1[x + N * y] != pattern2[(x - dx) + N * (y - dy)]) 
                        return false;
            return true;
        }

        /// @brief 
        static int* pattern(std::function<int(int, int)> f, int N)
        {
            int* result = new int[N * N];
            for (int y = 0; y < N; y++) for (int x = 0; x < N; x++) 
                result[x + y * N] = f(x, y);
            return result;
        }

        /// @brief 
        static int* rotate(int* p, int N) 
        {
            return pattern([&p, &N](int x, int y){ return p[N - 1 - y + x * N]; }, N);
        }

        /// @brief 
        static int* reflect(int* p, int N)
        {
            return pattern([&p, &N](int x, int y){ return p[N - 1 - x + y * N]; }, N);
        }

        /// @brief 
        static long hash(int* p, int C, int N)
        {
            long result = 0, power = 1;
            for (int i = 0; i < N * N; i++)
            {
                // result += p[N * N - 1 - i] * power;
                result += p[i] * power;
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
            int* sample = new int[SX * SY];
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

            std::map<long, int> patternIndices;

            int C = colors.size();
            int xmax = periodicInput ? SX : SX - N + 1;
            int ymax = periodicInput ? SY : SY - N + 1;


            for (int y = 0; y < ymax; y++) for (int x = 0; x < xmax; x++)
            {
                int** ps = new int*[8];

                ps[0] = pattern([&sample, &SX, &SY, &x, &y](int dx, int dy){ return sample[(x + dx) % SX + ((y + dy) % SY) * SX]; }, N);
                ps[1] = reflect(ps[0], N);
                ps[2] = rotate(ps[0], N);
                ps[3] = reflect(ps[2], N);
                ps[4] = rotate(ps[2], N);
                ps[5] = reflect(ps[4], N);
                ps[6] = rotate(ps[4], N);
                ps[7] = reflect(ps[6], N);

                for (int k = 0; k < symmetry; k++)
                {
                    int* p = ps[k];
                    
                    long h = hash(p, C, N);
                    if (auto iterIndex = patternIndices.find(h); iterIndex != patternIndices.end())
                        weights[iterIndex->second] ++;
                    else
                    {
                        patternIndices.insert({h, weights.size()});
                        weights.push_back(1.0);
                        patterns.push_back(pattern([&p, &N](int x, int y){return p[x + y * N];}, N));
                    }
                }

                for (int k = 0; k < 8; k ++)
                    delete[] ps[k];
                delete[] ps;
            }
            delete[] sample;

            T = weights.size();
            this->ground = ground;

            propagator = new std::vector<int>*[4];
            for (int d = 0; d < 4; d++)
            {
                propagator[d] = new std::vector<int>[T];
                for (int t = 0; t < T; t++)
                {
                    // std::vector<int> list;
                    for (int t2 = 0; t2 < T; t2++) 
                        if (agrees(patterns[t], patterns[t2], dx[d], dy[d], N)) 
                            propagator[d][t].push_back(t2);
                }
            }
        }

        ~OverlappingModel() 
        {
            for (auto pattern : patterns)
                delete[] pattern;
        }

    public: void Save(std::string filename)
    {

        std::cout << "----- PATTERNS -----" << std::endl;

        int iii = 0;
        for (auto pattern : patterns)
        {
            for (int y = 0; y < N; y ++)
            {
                for (int x = 0; x < N; x ++)
                    switch (colors[pattern[x + y * N]])
                    {
                        case 1052688:  std::cout << "."; break;
                        case 10526880: std::cout << "█"; break;
                        case 1052832:  std::cout << "◙"; break;
                        default: break;
                    }
                std::cout << '\t' << iii << '\t' << hash(pattern, colors.size(), N) << std::endl;
            }
            std::cout << std::endl;
            iii ++;
        }

        std::cout << "----- PROPAGATOR -----" << std::endl;
        for (int d = 0; d < 4; d ++)
            for (int t = 0; t < T; t ++) 
            {
                for (auto A : propagator[d][t])
                { std::cout << "in " << d << " from " << t << " to " << A << std::endl;
                    for (int y = 0; y < N; y ++)
                    {
                        for (int x = 0; x < N; x ++)
                            switch (colors[patterns[t][x + y * N]])
                            {
                                case 1052688:  std::cout << "."; break;
                                case 10526880: std::cout << "█"; break;
                                case 1052832:  std::cout << "◙"; break;
                                default: break;
                            }
                        std::cout << ' ';
                        for (int x = 0; x < N; x ++)
                            switch (colors[patterns[A][x + y * N]])
                            {
                                case 1052688:  std::cout << "."; break;
                                case 10526880: std::cout << "█"; break;
                                case 1052832:  std::cout << "◙"; break;
                                default: break;
                            }
                        std::cout << std::endl;
                    }std::cout << std::endl;
                }
            }

                    // std::cout << d << ' ' << t << ' ' << A << std::endl;
         
        std::cout << "----- RESULT -----" << std::endl;

        for (int y = 0; y < MY; y++)
        {
            int dy = y < MY - N + 1 ? 0 : N - 1;
            for (int x = 0; x < MX; x++)
            {
                int dx = x < MX - N + 1 ? 0 : N - 1;
                if (observed[x - dx + (y - dy) * MX] == -1) std::cout << '*';
                else switch (colors[patterns[observed[x - dx + (y - dy) * MX]][dx + dy * N]])
                    {
                        case 1052688:  std::cout << "."; break;
                        case 10526880: std::cout << "█"; break;
                        case 1052832:  std::cout << "◙"; break;
                        default: break;
                    }
            }
            // std::cout << '\t';
            // for (int x = 0; x < MX; x++)
            // {
            //     int dx = x < MX - N + 1 ? 0 : N - 1;
            //     std::cout << observed[x - dx + (y - dy) * MX] << '\t';
            // }
            std::cout << std::endl;
        }


        // int[] bitmap = new int[MX * MY];
        // if (observed[0] >= 0)
        // {
        //     for (int y = 0; y < MY; y++)
        //     {
        //         int dy = y < MY - N + 1 ? 0 : N - 1;
        //         for (int x = 0; x < MX; x++)
        //         {
        //             int dx = x < MX - N + 1 ? 0 : N - 1;
        //             bitmap[x + y * MX] = colors[patterns[observed[x - dx + (y - dy) * MX]][dx + dy * N]];
        //         }
        //     }
        // }
        // else
        // {
        //     for (int i = 0; i < wave.Length; i++)
        //     {
        //         int contributors = 0, r = 0, g = 0, b = 0;
        //         int x = i % MX, y = i / MX;
        //         for (int dy = 0; dy < N; dy++) for (int dx = 0; dx < N; dx++)
        //             {
        //                 int sx = x - dx;
        //                 if (sx < 0) sx += MX;

        //                 int sy = y - dy;
        //                 if (sy < 0) sy += MY;

        //                 int s = sx + sy * MX;
        //                 if (!periodic && (sx + N > MX || sy + N > MY || sx < 0 || sy < 0)) continue;
        //                 for (int t = 0; t < T; t++) if (wave[s][t])
        //                     {
        //                         contributors++;
        //                         int argb = colors[patterns[t][dx + dy * N]];
        //                         r += (argb & 0xff0000) >> 16;
        //                         g += (argb & 0xff00) >> 8;
        //                         b += argb & 0xff;
        //                     }
        //             }
        //         bitmap[i] = unchecked((int)0xff000000 | ((r / contributors) << 16) | ((g / contributors) << 8) | b / contributors);
        //     }
        // }
        // BitmapHelper.SaveBitmap(bitmap, MX, MY, filename);
    }
};