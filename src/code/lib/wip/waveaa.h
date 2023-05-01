#pragma once


#include "random.h"
#include "array2d.h"
#include <vector>
#include <set>
#include <iterator>



class MapGenerator {


    private: // +++VARIABLES+++

        /// @brief dimention of pattern
        static int N;

        /// @brief dimention of result
        static int sizeX, sizeY;

        /// @brief amount of patterns
        static int T;
        

        /// @brief set of patterns from base array
        static std::vector<Array2D<int>> patterns;

        /// @brief valid neighbours checker [up/right/down/left][current T][valid T]
        static std::vector<std::vector<int>> propagator[4];

        /// @brief amount of unique values
        // static Array2D<WaveNode> wave;

        /// @brief final array
        static Array2D<int> result;


    private: // +++SUPPORT FUNCTIONS+++

        static bool agrees(Array2D<int> pattern1, Array2D<int> pattern2, int dx, int dy, int N) {
            int xmin = dx < 0 ?    0   : dx, 
                xmax = dx < 0 ? dx + N : N, 
                ymin = dy < 0 ?    0   : dy, 
                ymax = dy < 0 ? dy + N : N;
            for (int y = ymin; y < ymax; y++) 
                for (int x = xmin; x < xmax; x++) 
                    if (pattern1.get(x, y) != pattern2.get(x - dx, y - dy)) 
                        return false;
            return true;
        };


    private: // +++MAIN FUNCTIONS+++


        static void Init(Array2D<int> image) {

            // make patterns
            for (int imageX = 0; imageX < image.X; imageX ++) for (int imageY = 0; imageY < image.Y; imageX ++){

                Array2D<int> currentPattern(N, N);
                image.cut(currentPattern, imageX, imageY);

                for (int i = 0; i < 8; i ++){

                    // modify
                    if (i % 2) currentPattern.rotate(1);
                    else currentPattern.mirror(1);
                        
                    // enshure that is new
                    bool isNew = true;
                    for (auto _pattern : patterns)
                        if (_pattern == currentPattern) { isNew = false; break; }
                    if (!isNew) continue;

                    // add
                    patterns.push_back(currentPattern);
                }
            }

            // set T
            T = patterns.size();

            // fill propagator
            for (int d = 0; d < 4; d ++) {
                propagator[d] = std::vector<std::vector<int>>();
                for (int p1 = 0; p1 < T; p1 ++) {
                    propagator[d].push_back(std::vector<int>());
                    for (int p2 = 0; p2 < T; p2 ++) {

                        // validate
                        int dx, dy;
                        switch (d) {
                            case 0: // up
                                dx = 0;
                                dy = -1;
                                break;
                            case 1: // right
                                dx = 1;
                                dy = 0;
                                break;
                            case 2: // down
                                dx = 0;
                                dy = 1;
                                break;
                            case 3: // left
                                dx = -1;
                                dy = 0;
                                break;
                        }
                        
                        if (agrees(*std::next(patterns.begin(), p1), *std::next(patterns.begin(), p2), dx, dy, N))
                            propagator[d][p1].push_back(p2);
                    }
                }
            }
        }


        int GetNextNode(){
            static int observed[2];

            // make observed zero
            observed[0] = -1;
            observed[1] = -1;

            // Find a wave element with the minimal nonzero entropy.
            double min = 1E+4;
            for (int x = 0; x < wave.X; x ++) for (int y = 0; y < wave.Y; y ++) {

                // collapsed already
                if (wave.get(x, y).collapsed) continue;

                // Entropy Zero (error)
                if (wave.get(x, y).entropySize == 0) throw;

                // Select node by rolling doube dice for each node and sum with entropy size,
                // resulting value where integer part is entropy size and fraction part is roll result
                double _value = wave.get(x, y).entropySize + Random::Double();
                if (_value < min) {
                    min = _value;
                    observed[0] = x;
                    observed[1] = y;
                }
            }
            // No nodes to collapse due to ether program finish or error
            return observed;

            // roll for each node

            // return next node
        }


        void Observe() {
            // TODO

            // select result
        }


        void Propagate() {
            // TODO

            // ban invalid tiels
        }



    public: // +++FUNCTIONS+++

        static Array2D<int> Gen(Array2D<int> __processed_image, int __N, int __sizeX, int __sizeY) {

            // set variables

            N = __N;
            sizeX = __sizeX;
            sizeY = __sizeY;
            wave = Array2D<WaveNode>(sizeX, sizeY);
            result = Array2D<int>(sizeX, sizeY);



            Init(__processed_image);


            while (true) {
                WaveNode* nextNode = GetNextNode();
                if (nextNode == nullptr) break;
                Observe(nextNode);
                Propagate(nextNode);
            }

            Finish();


            return result;
        }
};

