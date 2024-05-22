#include "generator.h"

// TEMP
#include "utility/log.h"

#include <vector>

Map *GenerateMapTiles(Map *__map, Mapdata __mapdata);

Map *MakeMapFromMapdata(Mapdata *__mapdata);

Map *MakeMapFromFile(const char *__mapdataFileRelPath);