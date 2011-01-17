#ifndef MAPMERGE_MAP_H
#define MAPMERGE_MAP_H

typedef struct MapObject
{
    void *path;
    void *vars;
} MapObject;


typedef struct Tile
{
    MapObject area, turf;
    unsigned int numObjects;
    MapObject *objects;
} Tile;


typedef struct Map
{
    unsigned int width, height, levels;
    Tile data[];
} Map;

#endif
