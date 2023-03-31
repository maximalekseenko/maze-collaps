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

    private: // ----- private Functions -----
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

    public:

        template<typename T>
        static Array2D<T> Gen(Array2D<T> pattern, int N, Array2D<T>* outArray) {
            std::set<Array2D<T>> patterns = MakePatterns(pattern, N);

        }
};


#endif //WAVE_H