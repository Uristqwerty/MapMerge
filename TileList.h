#ifndef MAPMERGE_TILELIST_H
#define MAPMERGE_TILELIST_H

#include "map.h"

typedef struct TileList
{
    unsigned int numTiles, allocatedTiles;
    Tile *tiles;
} TileList;


TileList *newTileList();

#endif
