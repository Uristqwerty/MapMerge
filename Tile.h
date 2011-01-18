#ifndef MAPMERGE_TILE_H
#define MAPMERGE_TILE_H

#include <stdio.h>
#include "MapObject.h"


typedef struct Tile
{
    MapObject area, turf;
    unsigned int numObjects, allocatedObjects;
    MapObject *objects;
} Tile;

MapObject *Tile_getNewMapObject(Tile *this);

int Tile_isEqual(Tile *this, Tile *other);

void Tile_Write(Tile *this, FILE *out);


#endif
