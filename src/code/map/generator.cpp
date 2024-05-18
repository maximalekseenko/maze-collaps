#include "generator.h"

Mapdata *ProcessTiles(Mapdata *__mapdata);

Map *GenerateMapTiles(Map *__map, Mapdata __mapdata);

Map *MakeMapFromMapdata(Mapdata *__mapdata);

Map *MakeMapFromFile(const char *__mapdataFileRelPath);