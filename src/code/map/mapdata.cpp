#include "mapdata.h"

#include <vector>
#include "utility/log.h"
#include "direction.h"

Mapdata::Mapdata(const char *__path)
    : path(__path)
{
}

Mapdata::~Mapdata()
{

    if (this->arePatternsSetted)
        this->ClearData();
}

void Mapdata::ClearData()
{
    // clear data defined in Process
    if (!this->arePatternsSetted)
        Log::Out("Mapdata: undefining when not defined.", true);

    // delete this->patterns
    for (int _patternsIndex = 0; _patternsIndex < this->patternsAmount; _patternsIndex++)
        delete[] this->patterns[_patternsIndex];
    delete[] this->patterns;

    // delete this->patternsWeights
    delete[] this->patternsWeights;

    // TODO DELETE OTHER STUFF AAAAAAAAAAAAAAAAAAAAAAAAAA
    // AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    // AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    // AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    // AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    // AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    // AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    // AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    // AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    // AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    // AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    // AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    // AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
}

/// @brief Can __pattern1 exist in position from __pattern2 specified by __deltaX and __deltaY.
/// Both patterns have size of __size
static bool doPattensAgree(int *__pattern1, int *pattern2, int __directionI, int __size)
{
    int xmin = direction::X[__directionI] < 0 ? 0 : direction::X[__directionI],
        xmax = direction::X[__directionI] < 0 ? direction::X[__directionI] + __size : __size,
        ymin = direction::Y[__directionI] < 0 ? 0 : direction::Y[__directionI],
        ymax = direction::Y[__directionI] < 0 ? direction::Y[__directionI] + __size : __size;
    for (int y = ymin; y < ymax; y++)
        for (int x = xmin; x < xmax; x++)
            if (__pattern1[x + __size * y] != pattern2[(x - direction::X[__directionI]) + __size * (y - direction::Y[__directionI])])
                return false;
    return true;
}

void Mapdata::SetPatterns(int *__rawImageData, int __rawImageWidth, int __rawImageHeight)
{
    { // Make patterns from raw image.
        std::vector<int *> _foundPatterns;
        std::vector<double> _foundPatternsWeights;

        // Extract a pattern from each point on raw image.
        for (int _imageY = 0; _imageY < __rawImageHeight; _imageY++)
            for (int _imageX = 0; _imageX < __rawImageWidth; _imageX++)
            {
                /// @note deleted or saved at the end.
                int *_newPattern = new int[this->patternSize * this->patternSize];

                // Get a new pattern from image at a point.
                for (int _patternY = 0; _patternY < this->patternSize; _patternY++)
                    for (int _patternX = 0; _patternX < this->patternSize; _patternX++)
                        _newPattern[_patternX + _patternY * this->patternSize] = __rawImageData[(_patternX + _imageX) % __rawImageWidth + ((_patternY + _imageY) % __rawImageHeight) * __rawImageWidth];

                // Check if newly extracted pattern is actually new.
                int _newPatternIndexInFoundPatterns = -1;
                for (int _existingPatternI = 0; _existingPatternI < _foundPatterns.size(); _existingPatternI++)
                {
                    bool _isNew = false;
                    for (int _patternI = 0; _patternI < this->patternSize * this->patternSize; _patternI++)
                        if (_newPattern[_patternI] != _foundPatterns[_existingPatternI][_patternI])
                        {
                            _isNew = true;
                            break;
                        }

                    if (!_isNew)
                    {
                        _newPatternIndexInFoundPatterns = _existingPatternI;
                        break;
                    }
                }

                // If pattern is new, add it to hash.
                if (_newPatternIndexInFoundPatterns == -1)
                {
                    _foundPatterns.push_back(_newPattern);
                    _foundPatternsWeights.push_back(1);
                    /// @note _newPattern's data is used in _foundPatterns.
                }
                // Else delete new instance and increase weight of saved one.
                else
                {
                    delete[] _newPattern;
                    _foundPatternsWeights[_newPatternIndexInFoundPatterns] += this->rawPatternDuplicateWeight;
                }
            }

        // Make arrays.
        this->patternsAmount = _foundPatterns.size();
        this->patterns = new int *[this->patternsAmount];
        this->patternsWeights = new double[this->patternsAmount];

        // And fill them with data.
        for (int _patternIndex = 0; _patternIndex < _foundPatterns.size(); _patternIndex++)
        {
            this->patterns[_patternIndex] = _foundPatterns[_patternIndex];
            this->patternsWeights[_patternIndex] = _foundPatternsWeights[_patternIndex];
        }
    }

    { // Calculate agreements.
        this->patternsAgreementsAmount = new int *[this->patternsAmount];
        this->patternsAgreements = new int **[this->patternsAmount];

        // For each pattern1; each direction; check pattern2
        for (int _pattern1Index = 0; _pattern1Index < this->patternsAmount; _pattern1Index++)
        {
            this->patternsAgreementsAmount[_pattern1Index] = new int[4];
            this->patternsAgreements[_pattern1Index] = new int *[4];

            // Check directions.
            std::vector<int> _agreedPatternsFoundInDirection;
            for (int _directionI = 0; _directionI < 4; _directionI++)
            {
                _agreedPatternsFoundInDirection.clear();

                // Check patters.
                for (int _pattern2Index = 0; _pattern2Index < this->patternsAmount; _pattern2Index++)
                {
                    // Check for agreements.
                    if (doPattensAgree(
                            this->patterns[_pattern1Index],
                            this->patterns[_pattern2Index],
                            _directionI,
                            this->patternSize))
                        _agreedPatternsFoundInDirection.push_back(_pattern2Index);
                }

                // Save agreements.
                this->patternsAgreementsAmount[_pattern1Index][_directionI] = _agreedPatternsFoundInDirection.size();
                this->patternsAgreements[_pattern1Index][_directionI] = new int[_agreedPatternsFoundInDirection.size()];
                for (int _agreedPatternsFoundInDirectionIndex = 0;
                     _agreedPatternsFoundInDirectionIndex < _agreedPatternsFoundInDirection.size();
                     _agreedPatternsFoundInDirectionIndex++)
                    this->patternsAgreements[_pattern1Index][_directionI][_agreedPatternsFoundInDirectionIndex] = _agreedPatternsFoundInDirection[_agreedPatternsFoundInDirectionIndex];
            }
        }
    }

    // Finish
    this->arePatternsSetted = true;
}

int Mapdata::GetPatternsAmount()
{
    if (!this->arePatternsSetted)
        Log::Out("Attempt at use of unfinished mapdata.", true);

    return this->patternsAmount;
}

int *Mapdata::GetPattern(int __patternIndex)
{
    if (!this->arePatternsSetted)
        Log::Out("Attempt at use of unfinished mapdata.", true);

    if (__patternIndex < 0 || __patternIndex >= this->patternsAmount)
        Log::Out("Pattern at index is not found in mapdata.", true);

    return this->patterns[__patternIndex];
}

double Mapdata::GetPatternWeight(int __patternIndex)
{
    if (!this->arePatternsSetted)
        Log::Out("Attempt at use of unfinished mapdata.", true);

    if (__patternIndex < 0 || __patternIndex >= this->patternsAmount)
        Log::Out("Pattern at index is not found in mapdata.", true);

    return this->patternsWeights[__patternIndex];
}

int Mapdata::GetPatternAgreementsAmount(int __patternIndex, int __direction)
{
    if (!this->arePatternsSetted)
        Log::Out("Attempt at use of unfinished mapdata.", true);

    if (__patternIndex < 0 || __patternIndex >= this->patternsAmount)
        Log::Out("Pattern at index is not found in mapdata.", true);

    if (__direction < 0 || __direction >= 4)
        Log::Out("Use of incorrect direction in mapdata.", true);

    return this->patternsAgreementsAmount[__patternIndex][__direction];
}

int *Mapdata::GetPatternAgreements(int __patternIndex, int __direction)
{
    if (!this->arePatternsSetted)
        Log::Out("Attempt at use of unfinished mapdata.", true);

    if (__patternIndex < 0 || __patternIndex >= this->patternsAmount)
        Log::Out("Pattern at index is not found in mapdata.", true);

    if (__direction < 0 || __direction >= 4)
        Log::Out("Use of incorrect direction in mapdata.", true);

    return this->patternsAgreements[__patternIndex][__direction];
}
