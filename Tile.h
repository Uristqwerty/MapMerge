#ifndef MAPMERGE_TILE_H
#define MAPMERGE_TILE_H

#include "MapObject.h"


typedef struct Tile
{
    MapObject area, turf;
    unsigned int numObjects, allocatedObjects;
    MapObject *objects;
} Tile;

MapObject *Tile_getNewMapObject(Tile *this);

#endif
