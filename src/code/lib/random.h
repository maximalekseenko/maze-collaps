#ifndef RANDOM_H
#define RANDOM_H

#ifndef _LIBCPP_RANDOM
    #include <random>
#endif //_LIBCPP_RANDOM

#ifndef _LIBCPP_MATH_H
    #include <math.h>
#endif // _LIBCPP_MATH_H



class Random{
    private:
        std::uniform_real_distribution<double> distrib;
        std::random_device dev;
        std::mt19937 gen;

    public:
        Random(int min, int max, int seed = -1) :
                    distrib(std::uniform_real_distribution<double>(min, max)),
                    dev(std::random_device()),
                    gen(std::mt19937(seed == -1? Random::dev() : seed)) {}

        double Double(){
            return distrib(gen);
        }

        int FromRange(int a, int b){
            if (a > b) std::swap(a, b);
            return round(distrib(gen) * (b - a + 1) + a - 0.5);
        }
};

#endif