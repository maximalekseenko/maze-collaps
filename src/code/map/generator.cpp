#include "generator.h"

// TEMP
#include "utility/log.h"

Mapdata *Generator::mapdata;
bool **Generator::wave;
int ***Generator::compatible;
std::uniform_real_distribution<double> Generator::dist;
std::vector<std::tuple<int, int>> Generator::stack;
double *Generator::weightLogWeights;
double Generator::sumOfWeights;
double Generator::sumOfWeightLogWeights;
double Generator::startingEntropy;
int *Generator::possiblePatternsLeft;
double *Generator::sumsOfWeights;
double *Generator::sumsOfWeightLogWeights;
double *Generator::entropies;

Map *Generator::Generate(Mapdata *__mapdata, int __seed, int __attemptsLimit, int __stepsLimit)
{
    // Initialize generator
    Generator::mapdata = __mapdata;
    Generator::Init();

    // Generate
    for (int _attemptsMade = 0; _attemptsMade < __attemptsLimit || __attemptsLimit < 0; _attemptsMade++)
        if (Generator::Run(__seed, __stepsLimit))
            break;

    // Save map;
    Map *_generatedMap = Generator::Save();

    // Clear runtime memory.
    Generator::Deinit();

    // Return
    return _generatedMap;
}

void Generator::Init()
{
    // core
    Generator::wave = new bool *[Generator::mapdata->GetMapSize()];
    Generator::compatible = new int **[Generator::mapdata->GetMapSize()];
    for (int _mapI = 0; _mapI < Generator::mapdata->GetMapSize(); _mapI++)
    {
        Generator::wave[_mapI] = new bool[Generator::mapdata->GetPatternsAmount()];
        Generator::compatible[_mapI] = new int *[Generator::mapdata->GetPatternsAmount()];
        for (int _patternIndex = 0; _patternIndex < Generator::mapdata->GetPatternsAmount(); _patternIndex++)
            Generator::compatible[_mapI][_patternIndex] = new int[4];
    }

    // pattern-oriented variables
    Generator::weightLogWeights = new double[Generator::mapdata->GetPatternsAmount()];
    Generator::sumOfWeights = 0;
    Generator::sumOfWeightLogWeights = 0;
    for (int _patternIndex = 0; _patternIndex < Generator::mapdata->GetPatternsAmount(); _patternIndex++)
    {
        Generator::weightLogWeights[_patternIndex] = Generator::mapdata->GetPatternWeight(_patternIndex) * std::log(Generator::mapdata->GetPatternWeight(_patternIndex));
        Generator::sumOfWeights += Generator::mapdata->GetPatternWeight(_patternIndex);
        Generator::sumOfWeightLogWeights += Generator::weightLogWeights[_patternIndex];
    }
    Generator::startingEntropy = std::log(Generator::sumOfWeights) - Generator::sumOfWeightLogWeights / Generator::sumOfWeights;

    // filled in clear on run
    Generator::possiblePatternsLeft = new int[mapdata->GetMapSize()];
    Generator::sumsOfWeights = new double[Generator::mapdata->GetMapSize()];
    Generator::sumsOfWeightLogWeights = new double[Generator::mapdata->GetMapSize()];
    Generator::entropies = new double[Generator::mapdata->GetMapSize()];

    // stack = std::vector<std::tuple<int, int>>;
}

void Generator::Deinit()
{
    if (Generator::wave != nullptr)
    {
        for (int _mapI = 0; _mapI < Generator::mapdata->GetMapSize(); _mapI++)
        {
            delete[] Generator::wave[_mapI];
            for (int _patternIndex = 0; _patternIndex < Generator::mapdata->GetPatternsAmount(); _patternIndex++)
                delete[] Generator::compatible[_mapI][_patternIndex];
            delete[] Generator::compatible[_mapI];
        }
        delete[] Generator::wave;
        delete[] Generator::compatible;
        delete[] Generator::possiblePatternsLeft;
        delete[] Generator::weightLogWeights;
        delete[] Generator::sumsOfWeights;
        delete[] Generator::sumsOfWeightLogWeights;
        delete[] Generator::entropies;
    }
}

Map *Generator::Save()
{
    // Define map
    Map *__newMap = new Map(
        Generator::mapdata->mapWidth,
        Generator::mapdata->mapHeight,
        "ADasD");

    // Fill map
    for (int _mapI = 0; _mapI < Generator::mapdata->GetMapSize(); _mapI++)
    {
        for (int _patternIndex = 0; _patternIndex < Generator::mapdata->GetPatternsAmount(); _patternIndex++)
            if (Generator::wave[_mapI][_patternIndex])
            {
                __newMap->SetTile(_mapI, Generator::mapdata->GetPattern(_patternIndex)[0]);
                break;
            }
    }

    // Return map
    return __newMap;
}

bool Generator::Run(int __seed, int __stepsLimit)
{
    Generator::Clear();

    std::mt19937 _gen(__seed < 0 ? std::random_device()() : __seed);

    for (int _l = 0; _l < __stepsLimit || __stepsLimit < 0; _l++)
    {

        if (int _nodeToObserveI = NextUnobservedNode(_gen); _nodeToObserveI >= 0)
        {
            Generator::Observe(_nodeToObserveI, _gen);
            bool success = Propagate();
            if (!success)
                return false;
        }
        else
        {
            return true;
        }
    }

    return true;
}

int Generator::NextUnobservedNode(std::mt19937 &gen)
{
    double min = 1E+4;
    int argmin = -1;
    for (int _nodeI = 0; _nodeI < Generator::mapdata->GetMapSize(); _nodeI++)
    {
        double entropy = Generator::entropies[_nodeI];
        if (Generator::possiblePatternsLeft[_nodeI] > 1 && entropy <= min)
        {
            double noise = 1E-6 * dist(gen);
            if (entropy + noise < min)
            {
                min = entropy + noise;
                argmin = _nodeI;
            }
        }
    }
    return argmin;
}

void Generator::Observe(int __nodeToObserve, std::mt19937 &gen)
{
    double _pool = Generator::dist(gen) * Generator::sumsOfWeights[__nodeToObserve];

    bool _isObservedPatternFound = false;

    // Ban all the patterns except one
    for (int _patternIndex = 0; _patternIndex < Generator::mapdata->GetPatternsAmount(); _patternIndex++)
    {
        // Decrease pool if pattern is not found
        if (!_isObservedPatternFound)
            _pool -= Generator::wave[__nodeToObserve][_patternIndex] ? Generator::mapdata->GetPatternWeight(_patternIndex) : 0.0;

        // If pattern is found
        if (!_isObservedPatternFound && _pool <= 0)
        {
            _isObservedPatternFound = true;
            continue;
        }

        // Else ban
        if (Generator::wave[__nodeToObserve][_patternIndex])
            Generator::Ban(__nodeToObserve, _patternIndex);
    }
}

bool Generator::Propagate()
{
    while (Generator::stack.size() > 0)
    {
        auto [_bannedPatternNodeI, _bannedPatternIndex] = stack.back();

        stack.pop_back();

        int _bannedPatternNodeX = _bannedPatternNodeI % Generator::mapdata->mapWidth;
        int _bannedPatternNodeY = _bannedPatternNodeI / Generator::mapdata->mapWidth;

        for (int _direction = 0; _direction < 4; _direction++)
        {
            int _nodeToCheckX = _bannedPatternNodeX + direction::X[_direction];
            int _nodeToCheckY = _bannedPatternNodeY + direction::Y[_direction];

            // fix for periodic
            if (_nodeToCheckX < 0)
                _nodeToCheckX += Generator::mapdata->mapWidth;
            else if (_nodeToCheckX >= Generator::mapdata->mapWidth)
                _nodeToCheckX -= Generator::mapdata->mapWidth;
            if (_nodeToCheckY < 0)
                _nodeToCheckY += Generator::mapdata->mapHeight;
            else if (_nodeToCheckY >= Generator::mapdata->mapHeight)
                _nodeToCheckY -= Generator::mapdata->mapHeight;

            int _nodeToCheckI = _nodeToCheckX + _nodeToCheckY * Generator::mapdata->mapWidth;
            int _agreementsInDirectionAmount = Generator::mapdata->GetPatternAgreementsAmount(_bannedPatternIndex, _direction);
            for (int _agreementsInDirectionIndex = 0;
                 _agreementsInDirectionIndex < _agreementsInDirectionAmount;
                 _agreementsInDirectionIndex++)
            {
                int _agreementPatternIndex = Generator::mapdata->GetPatternAgreement(_bannedPatternIndex, _direction, _agreementsInDirectionIndex);

                Generator::compatible[_nodeToCheckI][_agreementPatternIndex][direction::OPPOSITE[_direction]]--;
                if (Generator::compatible[_nodeToCheckI][_agreementPatternIndex][direction::OPPOSITE[_direction]] == 0)
                    Generator::Ban(_nodeToCheckI, _agreementPatternIndex);
            }
        }
    }

    return Generator::possiblePatternsLeft[0] > 0;
}

void Generator::Ban(int _bannedPatternNodeI, int _bannedPatternIndex)
{
    // remove pattern from node possibility
    Generator::wave[_bannedPatternNodeI][_bannedPatternIndex] = false;

    // clear compatables
    for (int _direction = 0; _direction < 4; _direction++)
        Generator::compatible[_bannedPatternNodeI][_bannedPatternIndex][_direction] = 0;

    Generator::stack.push_back({_bannedPatternNodeI, _bannedPatternIndex});

    Generator::possiblePatternsLeft[_bannedPatternNodeI] -= 1;
    Generator::sumsOfWeights[_bannedPatternNodeI] -= Generator::mapdata->GetPatternWeight(_bannedPatternIndex);
    Generator::sumsOfWeightLogWeights[_bannedPatternNodeI] -= Generator::weightLogWeights[_bannedPatternIndex];

    Generator::entropies[_bannedPatternNodeI] = std::log(Generator::sumsOfWeights[_bannedPatternNodeI]) - Generator::sumsOfWeightLogWeights[_bannedPatternNodeI] / Generator::sumsOfWeights[_bannedPatternNodeI];
}

void Generator::Clear()
{
    for (int _mapI = 0; _mapI < Generator::mapdata->GetMapSize(); _mapI++)
    {
        // core
        for (int _patternIndex = 0; _patternIndex < Generator::mapdata->GetPatternsAmount(); _patternIndex++)
        {
            Generator::wave[_mapI][_patternIndex] = true;
            for (int _direction = 0; _direction < 4; _direction++)
                Generator::compatible[_mapI][_patternIndex][_direction] = Generator::mapdata->GetPatternAgreementsAmount(_patternIndex, _direction);
        }

        // supportive
        Generator::possiblePatternsLeft[_mapI] = Generator::mapdata->GetPatternsAmount();
        Generator::sumsOfWeights[_mapI] = Generator::sumOfWeights;
        Generator::sumsOfWeightLogWeights[_mapI] = Generator::sumOfWeightLogWeights;
        Generator::entropies[_mapI] = Generator::startingEntropy;
    }
}
