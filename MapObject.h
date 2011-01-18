#ifndef MAPMERGE_MAPOBJECT_H
#define MAPMERGE_MAPOBJECT_H

#include <stdio.h>
#include "Path.h"


typedef struct MapObject
{
    Path *path;
    void *vars;
} MapObject;

void MapObject_Write(MapObject *this, FILE *file);


#endif
