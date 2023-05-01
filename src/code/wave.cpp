#include <iostream>


#include <random>
typedef std::mt19937 GENTYPE;
std::uniform_real_distribution<double> distrib(0, 1);



#include "wave.h"
#include "image.h"
#include "array2d.h"
#include <tuple>
#include <functional>



void MakePatterns(std::vector<Array2D<int>>& patterns, const char* fileName, int N)
{
    patterns.clear();

    // process image
    Array2D<int> image = Image::ParseImage(fileName);

    // make patterns
    Array2D<int> newPattern(N, N);
    
    for (int imageX = 0; imageX < image.X; imageX ++) for (int imageY = 0; imageY < image.Y; imageY ++)
    {

        // get new pattern
        image.cut(newPattern, imageX, imageY);

        // for each of 8 rotate&mirror states
        for (int iState = 0; iState < 8; iState ++)
        {

            // rotate or mirror
            if (iState % 2) newPattern.rotate(1);
            else newPattern.mirror(0);

            // add if new
            if (std::find(patterns.begin(), patterns.end(), &newPattern) == patterns.end())
                patterns.push_back(newPattern);
        }
    }
}



struct WaveNode
{
    int entropySize;
    bool* possiblePatterns;

    WaveNode() {}
    void Init(int T) 
    {
        entropySize = T;
        possiblePatterns = new bool[T];
        for (int i = 0; i < T; i ++) possiblePatterns[i] = true;
    }
    ~WaveNode() { delete[] possiblePatterns; }
};



void GetNextNode(int* observedNodePosition, Array2D<WaveNode>& wave, GENTYPE& gen)
{
    // set to "not found"
    observedNodePosition[0] = -1;
    observedNodePosition[1] = -1;
    
    // search
    double min = 1E+4;
    for (int x = 0; x < wave.X; x ++) for (int y = 0; y < wave.Y; y ++) {

        // Entropy One
        if (wave.get(x, y).entropySize == 1) throw;

        // Select node by rolling doube dice for each node and sum with entropy size,
        // resulting value where integer part is entropy size and fraction part is roll result
        double _value = wave.get(x, y).entropySize + distrib(gen);
        if (_value < min) {
            min = _value;
            observedNodePosition[0] = x;
            observedNodePosition[1] = y;
        }
    }
}



void Observe(int* observedNodePosition, bool* possiblePatterns, int pattensAmount, std::vector<std::tuple<int, int, int>>& hashedBannedPatterns, GENTYPE& gen)
{
    // select pattern to collape on
    int collapePattern = distrib(gen) * pattensAmount;

    // hash and ban ther patterns and node
    for (int iPattern = 0; iPattern < pattensAmount; iPattern ++) 
    {
        if (possiblePatterns[iPattern] != (iPattern == collapePattern))
        {
            // possiblePatterns[iPattern] = false;
            // hashedBannedPatterns.push_back({observedNodePosition[0], observedNodePosition[1], iPattern});
        }
    }
}



void Propagate()
{

}



void Generate(Array2D<int>& result, std::vector<Array2D<int>>& patterns, int seed = -1)
{
    // hased as [X, Y, Index]
    std::vector<std::tuple<int, int, int>> hashedBannedPatterns;

    Array2D<WaveNode> wave(result.X, result.Y);
    for (auto node : wave) node.Init(patterns.size());

    // random variables
    GENTYPE gen(seed == -1 ? std::random_device()() : seed);


    int observedNodePosition[2];

    while (true)
    {
        // get new node
        GetNextNode(observedNodePosition, wave, gen);

        // finish if not found
        if (observedNodePosition[0] == -1) break;

        // observe
        Observe(observedNodePosition, wave.get(observedNodePosition[0], observedNodePosition[1]).possiblePatterns, patterns.size(), hashedBannedPatterns, gen);

        // propagate
            // for each banned pattern
                // for each direction form node
                    // ban incompatable pattern
    }

    for (int iWave = 0; iWave < wave.len; iWave ++)
        for (int iPattern = 0; iPattern < patterns.size(); iPattern ++)
            if (wave.get(iWave).possiblePatterns[iPattern])
                result.set(iWave, patterns[iPattern].get(0, 0));
}



Array2D<int> GenerateMap(
    const char* fileName,
    int N,
    int resultX, int resultY
)
{
    // make patterns
    std::vector<Array2D<int>> patterns;
    MakePatterns(patterns, fileName, N);

    // generate
    Array2D<int> result(resultX, resultY);
    Generate(result, patterns);

    // return
    return result;   
}