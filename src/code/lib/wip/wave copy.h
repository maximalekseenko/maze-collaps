#ifndef WAVE_H
#define WAVE_H

#ifndef RANDOM_H
    #include "random.h"
#endif //RANDOM_H


#ifndef ARRAY2D
    #include "array2d.h"
#endif //ARRAY2D



#ifndef _LIBCPP_VECTOR
    #include <vector>
#endif //_LIBCPP_VECTOR


class Wave {
    private: // ----- private Classes -----
        /**
         * @brief Node in the wave matrix.
         * 
         * @tparam tileAmount amount of tiles found in pattern.
         */
        template<int tileAmount>
        class WaveNode {

            private: // ----- private Members -----
                Array2D<bool> entropyCollapse;


            public: // ----- public Membres -----
                int entropySize;


            public: // ----- Constructors and Destructors -----
                WaveNode() : 
                            entropySize(tileAmount), 
                            entropyCollapse(Array2D<bool, tileAmount, 1>) {
                    for (int _i = 0; _i < tileAmount; _i ++) entropyCollapse.set(i, true);
                }


            public: // ----- public Functions -----
                int Collapse(){
                    int collapeIn = Random::FromRange(1, entropySize);
                    int collapsedAt = -1;

                    // find final tile and ban all other
                    for (int _i = 0; _i < tileAmount; _i ++){
                        if (entropyCollapse.get(_i)){
                            collapeIn -= 1;
                            if (collapeIn == 0) collapsedAt = _i;
                            else BanTile(_i);
                        }
                    }

                    // cannot collapse
                    if (collapsedAt == -1) throw;

                    // save result
                    return collapsedAt;
                }

                void BanTile(int tile_i){
                    // Already Banned
                    if (!entropyCollapse.get(tile_i)) return;

                    // Ban
                    entropyCollapse.set(tile_i, false);
                    entropySize -= 1;
                }
        }; // WaveNode


    private: // ----- private Members -----
        Array2D<> pattern;
        int pattern_dim[2];

        Array2D result;
        int result_dim[2];

        Array2D<WaveNode> wave;
        int wave_dim[2];

        std::vector<int*> tilePatterns;
        int tile_n;

    private: // ----- private Functions -----


    public: // ----- Constructors and Destructors
        Wave(Array2D pattern){
            int tile_w_amount = pattern_dim[0] - tile_n + 1;
            int tile_h_amount = pattern_dim[1] - tile_n + 1;

            wave = Array2D<WaveNode<result_dim[0] * result_dim[1]>, nX, nY>();


            // add tile patterns
            tilePatterns.clear();
            for (int _px = 0; _px < tile_w_amount; _px ++) {
                for (int _py = 0; _py < tile_h_amount; _py ++) {
                    int *new_pattern = new int[tile_n * tile_n];
                    for (int _xx = 0; _xx < tile_n; _xx ++) for (int _yy = 0; _yy < tile_n; _yy ++)
                        new_pattern[_xx + _yy * tile_n] = pattern[(_xx + _px) + (_yy + _py) * pattern_dim[0]];


                    tilePatterns.push_back(new_pattern);
                }
            }
        }
    
    public:

        void Run(int result_w, int result_h, int N, int pattern_w, int patern_h, int *pattern){

            Wave::pattern_dim[0] = pattern_w;
            Wave::pattern_dim[1] = patern_h;
            Wave::pattern = pattern;

            Wave::result_dim[0] = result_w;
            Wave::result_dim[1] = result_h;
            Wave::result = new int[result_w * result_h];
            for (int _i = 0; _i < result_w * result_h; _i ++) Wave::result[_i] = -1;


            Wave::wave_dim[0] = pattern_dim[0] - tile_n + 1;
            Wave::wave_dim[1] = pattern_dim[1] - tile_n + 1;
            Wave::wave = new WaveNode*[result_dim[0] * result_dim[1]];
            for (int _i = 0; _i < result_dim[0] * result_dim[1]; _i ++)
                wave[_i] = new WaveNode(tile_w_amount * tile_h_amount);

            tile_n = N;

            Init();
            _Run();
        }

        void _Run(){
            int observe_i;
            int collpased_tile_i;

            while (true){
                
                // ----- Observation ----- //
                observe_i = Observe();
                if (observe_i == -1) break;
                // ----- Propagation ----- //
                Propagate(observe_i);
                    // propagate information gained on the previous observation step.
                // break;
            }
            // By now all the wave elements are either in a completely observed state (all the coefficients except one being zero) 
            // or in the contradictory state (all the coefficients being zero). In the first case return the output. 
            // In the second case finish the work without returning anything.

            std::cout << std::endl;

            for (int _x = 0; _x < result_dim[0]; _x ++){
                for (int _y = 0; _y < result_dim[1]; _y ++)
                    std::cout << result[_x + _y * result_dim[0]] << " ";
                std::cout << std::endl;
            }

            // Clean data
            // delete[] wave;
        }

        int Observe(){
                int observe_i = -1;

                // Find a wave element with the minimal nonzero entropy.
                double min = 1E+4;
                for (int _i = 0; _i < result_dim[0] * result_dim[1]; _i ++){

                    // Out of bounds
                    if (_i % result_dim[0] + tile_n > result_dim[0] || _i / result_dim[0] + tile_n > result_dim[1]) continue;

                    // Entropy Zero
                    if (wave[_i]->entropySize == 1) continue;

                    // Select node by rolling doube dice for each node and sum with entropy size,
                    // resulting value where integer part is entropy size and fraction part is roll result
                    double _value = wave[_i]->entropySize + 1E-6 * Random::Double();
                    if (_value < min) {
                        min = _value;
                        observe_i = _i;
                    }
                }

                // No nodes to collapse due to ether program finish or error
                return observe_i;
            
        }

        void Propagate(int node_i) {
            for (int tile_x = 0; tile_x < tile_n; tile_x ++)
                for (int tile_y = 0; tile_y < tile_n; tile_y ++)
                    result[node_i + tile_x + tile_y * tile_n] = tilePatterns[wave[node_i]->Collapse()][tile_x + tile_y * tile_n];
        }
};


#endif //WAVE_H