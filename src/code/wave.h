#ifndef WAVE_H
#define WAVE_H

#ifndef RANDOM_H
    #include "random.h"
#endif //RANDOM_H



class WaveNode{
    private:
        bool *entropyCollapse;
        bool isCollapsed = false;

    public:
        int value;
        int entropySize;


        WaveNode(){}
        WaveNode(int __tileAmount){

            entropyCollapse = new bool[__tileAmount];
            for (int _i = 0; _i < __tileAmount; _i ++) entropyCollapse[_i] = true;

            entropySize = __tileAmount;

            value = -1;
        }

        ~WaveNode(){
            delete entropyCollapse;
        }

        void Collapse(){
            if (isCollapsed) throw;

            int collapeIn = Random::FromRange(0, entropySize);
            int collapsedAt = 0;
            while (collapeIn != 0){
                if (entropyCollapse[collapsedAt])
                    collapeIn --;
                collapsedAt ++;
            }

            isCollapsed = true;
            entropySize = 1;
            value = collapsedAt;

        }
};

class Wave{

    public:
        Wave(int tile_w, int tile_h, int N, int pattern_w, int patern_h, int* pattern){

            /* Length of result (tile_w * tile_h) */
            int RES_LEN = tile_w * tile_h;
            // (optional) Augment pattern data with rotations and reflections. TODO
            int tile_w_amount = pattern_w - N + 1;
            int tile_h_amount = patern_h - N + 1;
            int TILES = (tile_w_amount * tile_h_amount);


            int** tilePatterns = new int*[TILES];


            // add tile patterns
            for (int _px = 0; _px < pattern_w - N + 1; _px ++) for (int _py = 0; _py < tile_h_amount; _py ++){
                tilePatterns[_px + _py * tile_w_amount] = new int[N * N];

                for (int _xx = 0; _xx < N; _xx ++) for (int _yy = 0; _yy < N; _yy ++)
                    tilePatterns[_px + _py * tile_w_amount][_xx + _yy * N] = pattern[(_xx + _px) + (_yy + _py) * pattern_w];
            }

            // ---------- Wave Init ---------- //

            WaveNode *wave = new WaveNode[RES_LEN];
            for (int _i = 0; _i < RES_LEN; _i ++)
                wave[_i] = WaveNode(TILES);

            
            // ---------- Wave Main Loop ---------- //
            int observe_i = -1;
            while (true){

                // ----- Observation ----- //
                observe_i = -1;

                // Find a wave element with the minimal nonzero entropy.
                double min = 1E+4;
                for (int _i = 0; _i < RES_LEN; _i ++){

                    // Out of bounds
                    if (_i % tile_w + N > tile_w || _i / tile_w + N > tile_h) continue;

                    // Entropy Zero
                    if (wave[_i].entropySize == 1) continue;

                    // Select node by rolling doube dice for each node and sum with entropy size,
                    // resulting value where integer part is entropy size and fraction part is roll result
                    double _value = wave[_i].entropySize + 1E-6 * Random::Double();
                    if (_value < min) {
                        min = _value;
                        observe_i = _i;
                    }
                }

                // No nodes to collapse due to ether program finish or error
                if (observe_i == -1) break;

                // Collapse this element into a definite state according to its coefficients and the distribution of NxN patterns in the input.
                wave[observe_i].Collapse();
            
                // Propagation: 
                    // propagate information gained on the previous observation step.
                // break;
            }
            // By now all the wave elements are either in a completely observed state (all the coefficients except one being zero) 
            // or in the contradictory state (all the coefficients being zero). In the first case return the output. 
            // In the second case finish the work without returning anything.

            for (int _x = 0; _x < tile_w; _x ++){
                for (int _y = 0; _y < tile_h; _y ++)
                    std::cout << wave[_x + _y * tile_w].value;
                std::cout << std::endl;
            }

            // Clean data
            // delete[] wave;
        }

};


#endif //WAVE_H