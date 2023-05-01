#ifndef WAVE_H
#define WAVE_H

#ifndef RANDOM_H
    #include "random.h"
#endif //RANDOM_H


#ifndef ARRAY2D_H
    #include "array2d.h"
#endif //ARRAY2D_H



#ifndef _LIBCPP_VECTOR
    #include <vector>
#endif //_LIBCPP_VECTOR

#ifndef _LIBCPP_SET
    #include <set>
#endif //_LIBCPP_SET


class Wave {
    private:
        class WaveNode {
            private: // ----- private Members -----
                bool* entropyCollapse;
                int tileAmount;


            public: // ----- public Membres -----
                int entropySize;
                bool collapsed;


            public: // ----- Constructors and Destructors -----
                WaveNode() : entropyCollapse(new bool[0]), collapsed(false) {}
                ~WaveNode() { delete[] entropyCollapse; }

            public: // ----- public Functions -----
                void Clear(int __tileAmount) {
                    entropySize = __tileAmount;
                    delete[] entropyCollapse;
                    entropyCollapse = new bool[__tileAmount];
                    tileAmount = __tileAmount;
                    for (int _i = 0; _i < __tileAmount; _i ++) entropyCollapse[_i] = true;
                }


                int Collapse(){
                    int collapeIn = Random::FromRange(1, entropySize);
                    int collapsedAt = -1;

                    // find final tile and ban all other
                    for (int _i = 0; _i < tileAmount; _i ++){
                        if (entropyCollapse[_i]){
                            collapeIn -= 1;
                            if (collapeIn == 0) collapsedAt = _i;
                            else BanTile(_i);
                        }
                    }

                    // cannot collapse
                    if (collapsedAt == -1) throw;

                    // save result
                    collapsed = true;
                    // entropySize = 1;
                    return collapsedAt;
                }

                void BanTile(int tile_i){
                    // Already Banned
                    if (!entropyCollapse[tile_i]) return;

                    // Ban
                    entropyCollapse[tile_i] = false;
                    entropySize -= 1;
                }
        }; // WaveNode

        template<typename T>
        static std::vector<Array2D<T>> MakePatterns(Array2D<T> pattern, int N) {
            std::vector<Array2D<T>> return_patterns;

            // collect all patters possible 
            for (int x = 0; x < pattern.X; x ++) for (int y = 0; y < pattern.Y; y ++) {
                Array2D<T> new_pattern(N, N);
                
                // collect new pattern for [x, y]
                for (int xx = 0; xx < N; xx ++) for (int yy = 0; yy < N; yy ++)
                    new_pattern.set(xx, yy, pattern.get(xx + x, yy + y));
                
                // enshure unique
                bool is_new = true;
                for (auto _pattern : return_patterns)
                    if (_pattern == new_pattern) { is_new = false; break; }
                if (!is_new) continue;

                // DELETE
                return_patterns.push_back(new_pattern); // normal
                // DELETE - END
                
                // add
                //  90*
                // new_pattern.rotate(1);
                // return_patterns.push_back(new_pattern); // normal
                // return_patterns.push_back(new_pattern.mirrorred(1)); // mirror
                // //  180*
                // new_pattern.rotate(1);
                // return_patterns.push_back(new_pattern); // normal
                // return_patterns.push_back(new_pattern.mirrorred(1)); // mirror
                // //  270*
                // new_pattern.rotate(1);
                // return_patterns.push_back(new_pattern); // normal
                // return_patterns.push_back(new_pattern.mirrorred(1)); // mirror
                // //  0*
                // new_pattern.rotate(1);
                // return_patterns.push_back(new_pattern); // normal
                // return_patterns.push_back(new_pattern.mirrorred(1)); // mirror
            }
            return return_patterns;
        }

        int NextUnobservedNode() {
            
        }

        template<typename T>
        static int* Observe(Array2D<T>& outArray, Array2D<WaveNode>& wave) {
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
        }

        template<typename T>   
        static void Propagate(Array2D<T>& outArray, Array2D<WaveNode>& wave, int* observed, std::vector<Array2D<T>> patterns, int N) {
            
            // collapse observed node and get final pattern
            Array2D<T> collapsed_pattern = *next(
                patterns.begin(), 
                wave.get(observed[0], observed[1]).Collapse()
            );

            // DELETE
            // std::cout << collapsed_pattern << std::endl;

            // iterate through collapsed pattern points
            for (int point_x = 0; point_x < N; point_x ++) for (int point_y = 0; point_y < N; point_y ++){

                // get value of that point
                T point_value = collapsed_pattern.get(point_x, point_y);

                // apply point value to outArray
                outArray.set(observed[0] + point_x, observed[1] + point_y, point_value);

                // check all patterns in all positons for if they can accept this change
                for (int pattern_i = 0; pattern_i < patterns.size(); pattern_i ++){

                    // check all possible positions for a new change on the pattern
                    for (int delta_x = 0; delta_x < N; delta_x ++) for (int delta_y = 0; delta_y < N; delta_y ++){

                        if (patterns[pattern_i].get(delta_x, delta_y) != point_value){
                            wave.get(
                                observed[0] + point_x - delta_x, 
                                observed[1] + point_y - delta_y
                                ).BanTile(pattern_i);
                        }
                    }
                }
            }
        }
    public:

        template<typename T>
        static bool Gen(Array2D<T> pattern, int N, Array2D<T>& outArray) {
            std::vector<Array2D<T>> patterns = MakePatterns(pattern, N);
            for (auto p : patterns) std::cout << p << std::endl;

            int gen_trys_left = 10;
            while (gen_trys_left > 0) {
                gen_trys_left --;
                try {

                    // make wave
                    Array2D<WaveNode> wave(outArray.X, outArray.Y);
                    for (int i = 0; i < wave.len; i ++) wave.get(i).Clear(patterns.size());

                    // observe-propagate loop untill error or entropy zero
                    while (true) {
                        int* observed = Observe(outArray, wave);
                        if (observed[0] == -1) break;
                        Propagate(outArray, wave, observed, patterns, N);

                        // DELETE BELLOW
                        std::cout << outArray << std::endl;
                        for (int y = 0; y < wave.Y; y ++){
                            for (int x = 0; x < wave.X; x ++)
                                std::cout << wave.get(x,y).entropySize << "\t";
                            std::cout << std::endl;
                        } std::cout << std::endl;
                        std::cout << std::endl;
                        // PURGE ABOVE
                    }

                    return true;

                } catch(const std::exception& e) { std::cerr << e.what() << '\n'; }
            }
            // fail
            return false;
        }
};


#endif //WAVE_H