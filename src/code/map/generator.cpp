#include "generator.h"

// TEMP
#include "utility/log.h"

Map *Generator::Generate(Mapdata *__mapdata)
{
    
}

Generator::Generator(Mapdata *__mapdata)
{
    this->mapdata = __mapdata;
}

Generator::~Generator()
{
    if (this->wave != nullptr)
    {
        for (int _mapI = 0; _mapI < this->mapdata->GetMapSize(); _mapI++)
        {
            delete[] wave[_mapI];
            for (int _patternIndex = 0; _patternIndex < this->mapdata->GetPatternsAmount(); _patternIndex++)
                delete[] compatible[_mapI][_patternIndex];
            delete[] compatible[_mapI];
        }
        delete[] wave;
        delete[] compatible;
    }
}

void Generator::Init()
{
    this->wave = new bool *[this->mapdata->GetMapSize()];
    this->compatible = new int **[this->mapdata->GetMapSize()];
    for (int _mapI = 0; _mapI < this->mapdata->GetMapSize(); _mapI++)
    {
        this->wave[_mapI] = new bool[this->mapdata->GetPatternsAmount()];
        this->compatible[_mapI] = new int *[this->mapdata->GetPatternsAmount()];
        for (int _patternIndex = 0; _patternIndex < this->mapdata->GetPatternsAmount(); _patternIndex++)
            this->compatible[_mapI][_patternIndex] = new int[4];
    }

    distribution = std::vector<double>(this->mapdata->GetPatternsAmount());

    weightLogWeights = std::vector<double>(this->mapdata->GetPatternsAmount());
    sumOfWeights = 0;
    sumOfWeightLogWeights = 0;

    for (int t = 0; t < this->mapdata->GetPatternsAmount(); t++)
    {
        weightLogWeights[t] = this->mapdata->GetPatternWeight(t) * std::log(this->mapdata->GetPatternWeight(t));
        sumOfWeights += this->mapdata->GetPatternWeight(t);
        sumOfWeightLogWeights += weightLogWeights[t];
    }

    startingEntropy = std::log(sumOfWeights) - sumOfWeightLogWeights / sumOfWeights;

    sumsOfOnes = std::vector<int>(this->mapdata->GetMapSize());
    sumsOfWeights = std::vector<double>(this->mapdata->GetMapSize());
    sumsOfWeightLogWeights = std::vector<double>(this->mapdata->GetMapSize());
    entropies = std::vector<double>(this->mapdata->GetMapSize());

    // stack = std::vector<std::tuple<int, int>>;
}

void Generator::Save(Map *&__map)
{
    __map = new Map(
        this->mapdata->mapWidth,
        this->mapdata->mapHeight,
        "ADasD");

    for (int _mapI = 0; _mapI < this->mapdata->GetMapSize(); _mapI++)
    {
        for (int _patternIndex = 0; _patternIndex < this->mapdata->GetPatternsAmount(); _patternIndex++)
            if (wave[_mapI][_patternIndex])
            {
                __map->SetTile(_mapI, this->mapdata->GetPattern(_patternIndex)[0]);
                break;
            }
    }
}

bool Generator::Run(int seed, int limit)
{
    if (this->wave == nullptr)
        Init();

    Clear();

    std::mt19937 gen(seed < 0 ? std::random_device()() : seed);

    for (int l = 0; l < limit || limit < 0; l++)
    {

        if (int _nodeToObserveI = NextUnobservedNode(gen); _nodeToObserveI >= 0)
        {
            Observe(_nodeToObserveI, gen);
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
    for (int i = 0; i < this->mapdata->GetMapSize(); i++)
    {
        int remainingValues = sumsOfOnes[i];
        double entropy = entropies[i];
        if (remainingValues > 1 && entropy <= min)
        {
            double noise = 1E-6 * dist(gen);
            if (entropy + noise < min)
            {
                min = entropy + noise;
                argmin = i;
            }
        }
    }
    return argmin;
}

void Generator::Observe(int __nodeToObserve, std::mt19937 &gen)
{
    double _randomDouble = dist(gen) * sumsOfWeights[__nodeToObserve];

    bool _isObservedPatternFound = false;
    for (int _patternIndex = 0; _patternIndex < this->mapdata->GetPatternsAmount(); _patternIndex++)
    {
        // decrease pool if pattern is not found
        if (!_isObservedPatternFound)
            _randomDouble -= this->wave[__nodeToObserve][_patternIndex] ? this->mapdata->GetPatternWeight(_patternIndex) : 0.0;

        // if pattern is found
        if (!_isObservedPatternFound && _randomDouble <= 0)
        {
            _isObservedPatternFound = true;
            continue;
        }

        // else ban
        if (this->wave[__nodeToObserve][_patternIndex])
            this->Ban(__nodeToObserve, _patternIndex);
    }
}

bool Generator::Propagate()
{
    while (this->stack.size() > 0)
    {
        auto [_bannedPatternNodeI, _bannedPatternIndex] = stack.back();

        stack.pop_back();

        int _bannedPatternNodeX = _bannedPatternNodeI % this->mapdata->mapWidth;
        int _bannedPatternNodeY = _bannedPatternNodeI / this->mapdata->mapWidth;

        for (int _direction = 0; _direction < 4; _direction++)
        {
            int _nodeToCheckX = _bannedPatternNodeX + direction::X[_direction];
            int _nodeToCheckY = _bannedPatternNodeY + direction::Y[_direction];

            // fix for periodic
            if (_nodeToCheckX < 0)
                _nodeToCheckX += this->mapdata->mapWidth;
            else if (_nodeToCheckX >= this->mapdata->mapWidth)
                _nodeToCheckX -= this->mapdata->mapWidth;
            if (_nodeToCheckY < 0)
                _nodeToCheckY += this->mapdata->mapHeight;
            else if (_nodeToCheckY >= this->mapdata->mapHeight)
                _nodeToCheckY -= this->mapdata->mapHeight;

            int _nodeToCheckI = _nodeToCheckX + _nodeToCheckY * this->mapdata->mapWidth;
            int _agreementsInDirectionAmount = this->mapdata->GetPatternAgreementsAmount(_bannedPatternIndex, _direction);
            for (int _agreementsInDirectionIndex = 0;
                 _agreementsInDirectionIndex < _agreementsInDirectionAmount;
                 _agreementsInDirectionIndex++)
            {
                int _agreementPatternIndex = this->mapdata->GetPatternAgreement(_bannedPatternIndex, _direction, _agreementsInDirectionIndex);

                this->compatible[_nodeToCheckI][_agreementPatternIndex][direction::OPPOSITE[_direction]]--;
                if (this->compatible[_nodeToCheckI][_agreementPatternIndex][direction::OPPOSITE[_direction]] == 0)
                    this->Ban(_nodeToCheckI, _agreementPatternIndex);
            }
        }
    }

    return sumsOfOnes[0] > 0;
}

void Generator::Ban(int _bannedPatternNodeI, int _bannedPatternIndex)
{
    // remove pattern from node possibility
    this->wave[_bannedPatternNodeI][_bannedPatternIndex] = false;

    // clear compatables
    for (int _direction = 0; _direction < 4; _direction++)
        this->compatible[_bannedPatternNodeI][_bannedPatternIndex][_direction] = 0;

    stack.push_back({_bannedPatternNodeI, _bannedPatternIndex});

    sumsOfOnes[_bannedPatternNodeI] -= 1;
    sumsOfWeights[_bannedPatternNodeI] -= this->mapdata->GetPatternWeight(_bannedPatternIndex);
    sumsOfWeightLogWeights[_bannedPatternNodeI] -= weightLogWeights[_bannedPatternIndex];

    double sum = sumsOfWeights[_bannedPatternNodeI];
    entropies[_bannedPatternNodeI] = std::log(sum) - sumsOfWeightLogWeights[_bannedPatternNodeI] / sum;
}

void Generator::Clear()
{
    for (int _mapI = 0; _mapI < this->mapdata->GetMapSize(); _mapI++)
    {
        for (int _patternIndex = 0; _patternIndex < this->mapdata->GetPatternsAmount(); _patternIndex++)
        {
            this->wave[_mapI][_patternIndex] = true;
            for (int _direction = 0; _direction < 4; _direction++)
                this->compatible[_mapI][_patternIndex][_direction] = this->mapdata->GetPatternAgreementsAmount(_patternIndex, _direction);
        }

        sumsOfOnes[_mapI] = this->mapdata->GetPatternsAmount();
        sumsOfWeights[_mapI] = sumOfWeights;
        sumsOfWeightLogWeights[_mapI] = sumOfWeightLogWeights;
        entropies[_mapI] = startingEntropy;
    }
}
