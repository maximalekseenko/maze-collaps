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


            public: // ----- Constructors and Destructors -----
                WaveNode() : entropyCollapse(new bool[0]) {}
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
                            if (collapeIn == 0) {
                                collapsedAt = _i;
                            }
                            BanTile(_i);
                        }
                    }

                    // cannot collapse
                    if (collapsedAt == -1) throw;

                    // save result
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
        static std::set<Array2D<T>> MakePatterns(Array2D<T> pattern, int N) {
            std::set<Array2D<T>> return_patterns;


            for (int x = 0; x < pattern.X - N + 1; x ++) for (int y = 0; y < pattern.Y - N + 1; y ++) {
                Array2D<T> new_pattern(N, N);
                
                // make new pattern for [x, y]
                for (int xx = 0; xx < N; xx ++) for (int yy = 0; yy < N; yy ++)
                    new_pattern.set(xx, yy, pattern.get(xx + x, yy + y));
                
                // add
                //  90*
                new_pattern.rotate(1);
                return_patterns.insert(new_pattern); // normal
                return_patterns.insert(new_pattern.mirrorred(1)); // mirror
                //  180*
                new_pattern.rotate(1);
                return_patterns.insert(new_pattern); // normal
                return_patterns.insert(new_pattern.mirrorred(1)); // mirror
                //  270*
                new_pattern.rotate(1);
                return_patterns.insert(new_pattern); // normal
                return_patterns.insert(new_pattern.mirrorred(1)); // mirror
                //  0*
                new_pattern.rotate(1);
                return_patterns.insert(new_pattern); // normal
                return_patterns.insert(new_pattern.mirrorred(1)); // mirror
            }
            return return_patterns;
        }

        template<typename T>
        static int* Observe(Array2D<T>& outArray, Array2D<WaveNode>& wave) {
            static int observed[2];
            observed[0] = -1;
            observed[1] = -1;

            // Find a wave element with the minimal nonzero entropy.
            double min = 1E+4;
            for (int x = 0; x < wave.X; x ++) for (int y = 0; y < wave.Y; y ++) {

                // Entropy Zero
                if (wave.get(x, y).entropySize == 0) 
                    continue;

                // Select node by rolling doube dice for each node and sum with entropy size,
                // resulting value where integer part is entropy size and fraction part is roll result
                double _value = wave.get(x, y).entropySize + 1E-6 * Random::Double();
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
        static void Propagate(Array2D<T>& outArray, Array2D<WaveNode>& wave, int* observed, std::set<Array2D<T>> patterns, int N, Array2D<bool>& outArrayChanged) {
            // collapse
            int collapsed_tile = wave.get(observed[0], observed[1]).Collapse();
            Array2D<T> pattern = *next(patterns.begin(), collapsed_tile);

            // apply patern to outArray
            for (int tile_x = 0; tile_x < N; tile_x ++) for (int tile_y = 0; tile_y < N; tile_y ++){
                T value = pattern.get(tile_x, tile_y);

                // apply to out
                outArray.set(observed[0] + tile_x, observed[1] + tile_y, value);
                outArrayChanged.set(observed[0] + tile_x, observed[1] + tile_y, true);
            }

            // ban failed tiles (fannot fit with current change)
            int pattern_i = -1;
            for (auto pattern_to_check : patterns){
                pattern_i ++;
                // check all possible tile positions
                for (int tile_dis_x = -N + 1; tile_dis_x < N; tile_dis_x ++) for (int tile_dis_y = -N + 1; tile_dis_y < N; tile_dis_y ++) {
                    // if out of bounds
                    if (       observed[0] + tile_dis_x < 0 
                            || observed[1] + tile_dis_y < 0 
                            || observed[0] + tile_dis_x >= wave.X 
                            || observed[1] + tile_dis_y >= wave.Y
                            ) continue;

                    // check if pattern is ok with the changes
                    for (int tile_x = 0; tile_x < N; tile_x ++) for (int tile_y = 0; tile_y < N; tile_y ++){
                        // if tile with such disposition is uneffected by current tile[x,y]
                        if (       -tile_dis_x + tile_x < 0 
                                || -tile_dis_y + tile_y < 0 
                                || -tile_dis_x + tile_x >= N 
                                || -tile_dis_y + tile_y >= N
                                ) continue;

                        if (pattern_to_check.get(-tile_dis_x + tile_x, -tile_dis_y + tile_y) != pattern.get(tile_x, tile_y)){
                            wave.get(observed[0] + tile_dis_x, observed[1] + tile_dis_y).BanTile(pattern_i);
                            break;
                        }
                    }
                }


            }
        }
    public:

        template<typename T>
        static bool Gen(Array2D<T> pattern, int N, Array2D<T>& outArray) {
            std::set<Array2D<T>> patterns = MakePatterns(pattern, N);

            int gen_trys_left = 10;
            while (gen_trys_left > 0) {
                gen_trys_left --;

                Array2D<bool> outArrayChanged(outArray.X, outArray.Y);

                // make wave
                Array2D<WaveNode> wave(outArray.X - N + 1, outArray.Y - N + 1);
                for (int i = 0; i < wave.len; i ++) wave.get(i).Clear(patterns.size());

                // observe-propagate loop untill error or entropy zero
                while (true) {
                    int* observed = Observe(outArray, wave);
                    if (observed[0] == -1) break;
                    Propagate(outArray, wave, observed, patterns, N, outArrayChanged);
                }

                bool is_final = true;
                for (int i = 0; i < outArrayChanged.len; i ++)
                    if (!outArrayChanged.get(i)){
                        is_final = false;
                        break;
                    }

                // success
                if (is_final) return true;
            }
            // fail
            return false;
        }
};


#endif //WAVE_H