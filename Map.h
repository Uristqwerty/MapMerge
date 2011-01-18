#ifndef MAPMERGE_MAP_H
#define MAPMERGE_MAP_H

#include "Tile.h"


typedef struct Map
{
    unsigned int width, height, levels;
    Tile data[];
} Map;


#endif
