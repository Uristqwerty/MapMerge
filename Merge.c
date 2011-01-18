#include <stdlib.h>
#include <stdio.h>
#include "Map.h"
#include "Merge.h"

Map *MergeMaps(Map *ancestor, Map *current, Map *merge)
{
    if(!ancestor || !current || !merge)
    {
        printf("Could not merge maps; at least one of the passed Map pointers was null. Talk to a MapMerge dev about it, this is an internal error.\n");
        return NULL;
    }

    Map *map = newMap();
    if(!map)
      return NULL;

    if(current->width > merge->width)
      map->width = current->width;
    else
      map->width = merge->width;

    if(current->height > merge->height)
      map->height = current->height;
    else
      map->height = merge->height;

    if(current->levels > merge->levels)
      map->levels = current->levels;
    else
      map->levels = merge->levels;

    map = Map_Resized(map);
    if(!map)
      return NULL;

    printf("Could not merge maps; function is unimplemented.\n");
    return NULL;
}

