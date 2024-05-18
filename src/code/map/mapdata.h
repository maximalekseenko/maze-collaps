#ifndef __MAP_MAPDATA_H
#define __MAP_MAPDATA_H

struct Mapdata
{
    Mapdata();
    ~Mapdata();

    // ---------- raw stuff ----------
    int rawImageWidth;
    int rawImageHeight;
    int* rawImage;

    // ---------- processed stuff ----------
    int patternsAmount;
    int** patterns;
    int* patternWeights;

    // ---------- more or less constant members ----------
    const char* name;
    int patternSize;
    int mapWidth;
    int mapHeight;
};

#endif // __MAP_MAPDATA_H