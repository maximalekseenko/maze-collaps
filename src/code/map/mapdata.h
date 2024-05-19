#ifndef __MAP_MAPDATA_H
#define __MAP_MAPDATA_H

class Mapdata
{
public:
    Mapdata();
    ~Mapdata();

public: // TODO: MOVE TO GENERRATOR
    void ProcessRawImage(int *__rawImageData, int __rawImageWidth, int __rawImageHeight);

public:
    // ---------- processed stuff ----------
    bool arePatternsProcessed;
    int patternsAmount;
    int **patterns;
    double *patternWeights;

    // ---------- more or less constant members ----------
    const char *name;

    int patternSize = 3;
    int mapWidth = 16;
    int mapHeight = 32;

    // ---------- weights ----------
    double rawPatternDuplicateWeight = 1;
};

#endif // __MAP_MAPDATA_H