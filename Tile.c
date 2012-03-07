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

    if(this->numObjects >= this->allocatedObjects)
    {
        MapObject *objects = realloc(this->objects, sizeof(MapObject) * (this->allocatedObjects + 8));
        if(!objects)
        {
            printf("Could not get new MapObject from Tile; malloc failure. Talk to a MapMerge dev about it, this is an internal error.\n");
            return NULL;
        }

        this->objects = objects;
        this->allocatedObjects += 8;
    }

    MapObject *mapObject = &this->objects[this->numObjects];
    this->numObjects++;

    mapObject->path = NULL;
    mapObject->vars = NULL;

    return mapObject;
}

int Tile_isEqual(Tile *this, Tile *other)
{

    if(!this || !other)
      return 0;

    if(this->numObjects != other->numObjects)
      return 0;

    if(!MapObject_isEqual(&this->area, &other->area))
      return 0;

    if(!MapObject_isEqual(&this->turf, &other->turf))
      return 0;

    int i;
    for(i=0; i<this->numObjects; i++)
      if(!MapObject_isEqual(&this->objects[i], &other->objects[i]))
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

int Tile_Init(Tile *this)
{
    if(!this)
    {
        printf("Could not init Tile; null Tile. Talk to a MapMerge dev about it, this is an internal error.\n");
        return 0;
    }

    this->area.path = NULL;
    this->area.vars = NULL;
    this->turf.path = NULL;
    this->turf.vars = NULL;
    this->area.path = NULL;

    this->objects = NULL;
    this->numObjects = 0;
    this->allocatedObjects = 0;

    return 1;
}



