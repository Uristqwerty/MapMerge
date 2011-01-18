#include <stdlib.h>
#include <stdio.h>
#include "Map.h"

Map *newMap()
{
    Map *map = malloc(sizeof(Map));
    if(!map)
    {
        printf("Could not create map.\n");
        return NULL;
    }

    map->width = 0;
    map->height = 0;
    map->levels = 0;
    return map;
}

void Map_SetTile(Map *this, Tile *tile, int x, int y, int z)
{
    if(!this || !tile || x < 0 || y < 0 || z < 0 || x >= this->width || y >= this->height || z >= this->levels)
      return;

    this->data[z*this->width*this->height + y*this->width + x] = *tile;
}

Map *Map_Resized(Map *this)
{
    if(!this)
      return NULL;

    this = realloc(this, sizeof(Map) + (sizeof(Tile) * this->width * this->height * this->levels));
    if(!this)
    {
        printf("Could not resize map; aborting\n");
        return NULL;
    }

    return this;
}

Tile *Map_GetTile(Map *this, int x, int y, int z)
{
    if(!this || x < 0 || y < 0 || z < 0 || x >= this->width || y >= this->height || z >= this->levels)
      return NULL;

    return &this->data[z*this->width*this->height + y*this->width + x];
}
