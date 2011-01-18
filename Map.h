#ifndef MAPMERGE_MAP_H
#define MAPMERGE_MAP_H

#include "Tile.h"


typedef struct Map
{
    unsigned int width, height, levels;
    Tile data[];
} Map;


Map *newMap();
Map *Map_Resized(Map *this);

void Map_SetTile(Map *this, Tile *tile, int x, int y, int z);

Tile *Map_GetTile(Map *this, int x, int y, int z);

#endif
