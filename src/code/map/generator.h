#ifndef __MAP_GENERATOR_H
#define __MAP_GENERATOR_H

#include "./mapdata.h"
#include "./map.h"
#include "./reader.h"

namespace Generator
{
    Map *GenerateMapTiles(Map *__map, Mapdata __mapdata);

    /// @brief Generates a map based on parameters provided.
    /// @param __mapdata Mapdata with settings for generation.
    /// @return Map filled with tiles.
    Map *GenerateMap(Mapdata *__mapdata);

    /// @brief Generates a map based on parameters provided.
    /// @param __mapdataFileRelPath Relative path to file containing
    ///                             mapdata with settings for generation.
    /// @return Map filled with tiles.
    Map *GenerateMap(const char *__mapdataFileRelPath);
}

#endif // __MAP_GENERATOR_H