#ifndef MAPMERGE_MAPOBJECT_H
#define MAPMERGE_MAPOBJECT_H

#include <stdio.h>
#include "Path.h"
#include "Vars.h"


typedef struct MapObject
{
    Path *path;
    Vars *vars;
} MapObject;

void MapObject_Write(MapObject *this, FILE *file);

int MapObject_isEqual(MapObject *this, MapObject *other);


#endif
