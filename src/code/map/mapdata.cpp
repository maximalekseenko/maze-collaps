#include "mapdata.h"

#include <vector>

Mapdata::Mapdata()
{
}

Mapdata::~Mapdata()
{
}

void Mapdata::ProcessRawImage(int *__rawImageData, int __rawImageWidth, int __rawImageHeight)
{
    std::vector<int *> _foundPatterns;
    std::vector<double> _foundPatternsWeights;

    // find patterns
    for (int _imageY = 0; _imageY < __rawImageHeight; _imageY++)
        for (int _imageX = 0; _imageX < __rawImageWidth; _imageX++)
        {
            /// @note deleted or saved at the end
            int *_newPattern = new int[this->patternSize * this->patternSize];

            // get pattern
            for (int _patternY = 0; _patternY < this->patternSize; _patternY++)
                for (int _patternX = 0; _patternX < this->patternSize; _patternX++)
                    _newPattern[_patternX + _patternY * this->patternSize] = __rawImageData[(_patternX + _imageX) % __rawImageWidth + ((_patternY + _imageY) % __rawImageHeight) * __rawImageWidth];

            // check if new
            int _newPatternIndexInFoundPatterns = -1;
            for (int _existingPatternI = 0; _existingPatternI < _foundPatterns.size(); _existingPatternI++)
            {
                bool _isNew = true;
                for (int _patternI = 0; _patternI < this->patternSize * this->patternSize; _patternI++)
                    if (_newPattern[_patternI] != _foundPatterns[_existingPatternI][_patternI])
                    {
                        _isNew = false;
                        break;
                    }

                if (!_isNew)
                    continue;

                // found dublicate
                _newPatternIndexInFoundPatterns = _existingPatternI;
                break;
            }

            // add new or increase weight of existing
            if (_newPatternIndexInFoundPatterns == -1)
            {
                _foundPatterns.push_back(_newPattern);
                _foundPatternsWeights.push_back(1);
                // note: _newPattern's data is used in _foundPatterns
            }
            else
            {
                delete[] _newPattern;
                _foundPatternsWeights[_newPatternIndexInFoundPatterns] += this->rawPatternDuplicateWeight;
            }
        }

    // save found patterns
    this->patternsAmount = _foundPatterns.size();
    this->patterns = new int *[this->patternsAmount];
    this->patternWeights = new double[this->patternsAmount];
    for (int _patternIndex = 0; _patternIndex < _foundPatterns.size(); _patternIndex++)
    {
        this->patterns[_patternIndex] = _foundPatterns[_patternIndex];
        this->patternWeights[_patternIndex] = _foundPatternsWeights[_patternIndex];
    }
}