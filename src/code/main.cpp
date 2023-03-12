#include <iostream>
#include <vector>
#include <random>
#include <math.h>


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


class WaveNode{
    bool *entropyCollapse;
    int entropySize;

    WaveNode(int __tileAmount){

        this->entropyCollapse = new bool[__tileAmount];
        for (int _i = 0; _i < __tileAmount; _i ++) this->entropyCollapse[_i] = true;

        this->entropySize = __tileAmount;
    }

    ~WaveNode(){
        delete this->entropyCollapse;
    }
};


void Wave(int width, int height, int N){

   std::uniform_real_distribution<double> unif(0, 1);
   std::random_device dev;
   std::mt19937 re(dev());


    /* Length of result (width * height) */
    int RES_LEN = width * height;
    // Read the input bitmap and count NxN patterns. TODO
    // (optional) Augment pattern data with rotations and reflections. TODO
    int* PATTERN = new int[16]{
        0, 1, 0, 0,
        0, 1, 0, 0,
        1, 1, 1, 1,
        0, 1, 0, 0};
    int PATTERN_W = 4;
    int PATTERN_H = 4;
    int TILES = ((PATTERN_W - N + 1) * (PATTERN_H - N + 1));


    int** tilePatterns = new int*[TILES];


    // add tile patterns
    for (int _px = 0; _px < PATTERN_W - N + 1; _px ++) for (int _py = 0; _py < PATTERN_H - N + 1; _py ++){
        tilePatterns[_px + _py * (PATTERN_W - N + 1)] = new int[N * N];

        for (int _xx = 0; _xx < N; _xx ++) for (int _yy = 0; _yy < N; _yy ++)
            tilePatterns[_px + _py * (PATTERN_W - N + 1)][_xx + _yy * N] = PATTERN[(_xx + _px) + (_yy + _py) * PATTERN_W];
    }

    // ---------- Wave Init ---------- //

    /* Matrix[width * height],
    where each element is a list of booleans[length of amount of tiles], 
    that represents node possibility to be specific tile. */
    bool **wave = new bool*[RES_LEN];
    

    /* Matrix[width * height],
    where each element is an amount of possible states for this node. */
    int *entropySize = new int[RES_LEN];

    // init default values
    for (int node_i = 0; node_i < RES_LEN; node_i++){

        // all wave nodes
        wave[node_i] = new bool[TILES];
        for (int _t = 0; _t < TILES; _t++) wave[node_i][_t] = true;

        entropySize[node_i] = TILES;
    }
    
    // Repeat the following steps:
    int observe_i = -1;
    while (true){
    // Observation:

        observe_i = -1;

        // Find a wave element with the minimal nonzero entropy.
        double min = 1E+4;
        for (int _i = 0; _i < RES_LEN; _i ++){

            // Out of bounds
            if (_i % width + N > width || _i / width + N > height) continue;

            // entropy zero
            if (entropySize[_i] == 1) continue;

            // select by random DOTO
            double noise = 1E-6 * unif(re);
            if (entropySize[_i] + noise < min) {
                min = entropySize[_i] + noise;
                observe_i = _i;
            }
        }
        // std::cout << observe_i;

        // If there is no such elements (if all elements have zero or undefined entropy) then break the cycle (4) and go to step (5).
        if (observe_i == -1) break;

        // Collapse this element into a definite state according to its coefficients and the distribution of NxN patterns in the input.
        int tile = round(unif(re) * entropySize[observe_i]);
        entropySize[observe_i] -= 1;
        wave[observe_i][tile] = false;
    
    // Propagation: 
        // propagate information gained on the previous observation step.
        break;
    }
    // By now all the wave elements are either in a completely observed state (all the coefficients except one being zero) 
    // or in the contradictory state (all the coefficients being zero). In the first case return the output. 
    // In the second case finish the work without returning anything.

    // Clean data
    for (int _i = 0; _i < RES_LEN; _i++) delete wave[_i];
    delete[] wave;
}




int main()
{
    // ProcessImage("dungeon1.png");
    // Wave w = Wave("dungeon1.png", 4);
    Wave(12, 12, 3);
}
