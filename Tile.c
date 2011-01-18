#include <stdlib.h>
#include <stdio.h>
#include "MapObject.h"
#include "Tile.h"

MapObject *Tile_getNewMapObject(Tile *this)
{
    if(!this)
    {
        printf("Could not get new MapObject from Tile; null Tile. Talk to a MapMerge dev about it, this is an internal error.\n");
        return NULL;
    }

    printf("Could not get new MapObject from Tile; unimplemented\n");
    return NULL;
}
