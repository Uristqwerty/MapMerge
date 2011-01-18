#ifndef MAPMERGE_MAPOBJECT_H
#define MAPMERGE_MAPOBJECT_H

#include "Path.h"


typedef struct MapObject
{
    Path *path;
    void *vars;
} MapObject;


#endif
