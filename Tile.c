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

int Tile_isEqual(Tile *this, Tile *other)
{

    if(!this || !other)
      return 0;

    if(this->numObjects != other->numObjects)
      return 0;

    if(this->area.path != other->area.path)
      return 0;

    if(this->turf.path != other->turf.path)
      return 0;

    return 1;
}

void Tile_Write(Tile *this, FILE *file)
{
    if(!this)
    {
        printf("Could not write Tile; null Tile. Talk to a MapMerge dev about it, this is an internal error.\n");
        return;
    }
    if(!file)
    {
        printf("Could not write Tile; null file. Talk to a MapMerge dev about it, this is an internal error.\n");
        return;
    }

    int i;
    for(i=0; i<this->numObjects; i++)
    {
        MapObject_Write(&this->objects[i], file);
        fprintf(file, ",");
    }

    MapObject_Write(&this->turf, file);
    fprintf(file, ",");
    MapObject_Write(&this->area, file);
}
