#ifndef __MAP_MAPDATA_H
#define __MAP_MAPDATA_H

#include <vector>

class Mapdata
{
public:
    Mapdata(const char *__path);
    ~Mapdata();

public:
    void SetPatterns(int *__rawImageData, int __rawImageWidth, int __rawImageHeight);
    void ClearData();

private:
    /// @brief True if this Mapdata has patterns defined.
    bool arePatternsSetted = false;

    /// @brief Amount of patterns in this mapdata.
    /// @warning Must be defined if `arePatternsDefined` is `true`.
    int patternsAmount = 0;

    /// @brief Array of patterns of this mapdata.
    /// @note Array sizes are [`patternsAmount`][`patternSize`].
    /// @warning Must be defined if `arePatternsDefined` is `true`.
    /// @warning Do not access directly, use `GetPattern` and `DefinePatterns` instead.
    int **patterns = nullptr;

    /// @brief Array of patterns' weights.
    /// @note Array sizes are [`patternsAmount`].
    /// @warning Must be defined if `arePatternsDefined` is `true`.
    /// @warning Do not access directly, use `GetPatternWeight` and `DefinePatterns` instead.
    double *patternsWeights = nullptr;

    int **patternsAgreementsAmount;
    int ***patternsAgreements;

public:
    int GetMapSize();
    int GetPatternsAmount();

    /// @brief Gets the pattern at index.
    /// @param __patternIndex Index of a pattern.
    /// @return Pointer on a pattern at index
    /// @throws Throws an error if patterns are not defined.
    /// @throws Throws an error if there is no pattern on index provided.
    int *GetPattern(int __patternIndex);

    /// @brief Gets weight of the pattern at index.
    /// @param __patternIndex Index of a pattern.
    /// @return Weight of a pattern at index.
    /// @throws Throws an error if patterns are not defined.
    /// @throws Throws an error if there is no pattern on index provided.
    double GetPatternWeight(int __patternIndex);

    int GetPatternAgreementsAmount(int __patternIndex, int __direction);
    int GetPatternAgreement(int __patternIndex, int __direction, int __agreementIndex);

    // ---------- more or less constant members ----------
    const char *path;

    int patternSize = 3;
    int mapWidth = 32;
    int mapHeight = 16;

    // ---------- weights ----------
    double rawPatternDuplicateWeight = 1;
};

#endif // __MAP_MAPDATA_H