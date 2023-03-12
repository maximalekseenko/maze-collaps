#ifndef WAVE_H
#define WAVE_H

#ifndef RANDOM_H
    #include "random.h"
#endif //RANDOM_H


#ifndef _LIBCPP_VECTOR
    #include "vector"
#endif //_LIBCPP_VECTOR





class WaveNode{
    private:
        bool *entropyCollapse;
        bool isCollapsed = false;

    public:
        int value;
        int entropySize;


        WaveNode(){}
        WaveNode(int tileAmount) {
            entropyCollapse = new bool[tileAmount];
            for (int _i = 0; _i < tileAmount; _i ++) entropyCollapse[_i] = true;
            entropySize = tileAmount;
            value = -1;
        }

        ~WaveNode() { delete[] entropyCollapse; }

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
    private:
        int* pattern;
        int pattern_dim[2];
        int result_dim[2];
        int tile_n;

        WaveNode **wave;

        std::vector<int*> tilePatterns;

        void Init(){
            int tile_w_amount = pattern_dim[0] - tile_n + 1;
            int tile_h_amount = pattern_dim[1] - tile_n + 1;

            wave = new WaveNode*[result_dim[0] * result_dim[1]];
            for (int _i = 0; _i < result_dim[0] * result_dim[1]; _i ++)
                wave[_i] = new WaveNode(tile_w_amount * tile_h_amount);


            tilePatterns.clear();

            // add tile patterns
            for (int _px = 0; _px < tile_w_amount; _px ++) {
                for (int _py = 0; _py < tile_h_amount; _py ++) {
                    int new_pattern[tile_n * tile_n];
                    for (int _xx = 0; _xx < tile_n; _xx ++) for (int _yy = 0; _yy < tile_n; _yy ++)
                        new_pattern[_xx + _yy * tile_n] = pattern[(_xx + _px) + (_yy + _py) * pattern_dim[0]];
                    tilePatterns.push_back(new_pattern);
                }
            }
        }


    public:
        Wave(int result_w, int result_h, int N, int pattern_w, int patern_h, int* pattern){
            pattern_dim[0] = pattern_w;
            pattern_dim[1] = patern_h;

            result_dim[0] = result_w;
            result_dim[1] = result_h;

            tile_n = N;

            Init();
            Run();
        }

        void Run(){
            while (true){
                
                if (!Observe()) break;
                // ----- Observation ----- //
                // Propagation: 
                    // propagate information gained on the previous observation step.
                // break;
            }
            // By now all the wave elements are either in a completely observed state (all the coefficients except one being zero) 
            // or in the contradictory state (all the coefficients being zero). In the first case return the output. 
            // In the second case finish the work without returning anything.

            for (int _x = 0; _x < result_dim[0]; _x ++){
                for (int _y = 0; _y < result_dim[1]; _y ++)
                    std::cout << wave[_x + _y * result_dim[0]]->value << " ";
                std::cout << std::endl;
            }

            // Clean data
            // delete[] wave;
        }

        bool Observe(){
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
                if (observe_i == -1) return false;

                // Collapse this element into a definite state according to its coefficients and the distribution of NxN patterns in the input.
                wave[observe_i]->Collapse();

                return true;
            
        }
};


#endif //WAVE_H