#include <stdlib.h>
#include <stdio.h>
#include "Map.h"
#include "Merge.h"


Tile* MergeTiles(Tile *current, Tile *merge, Tile *ancestor, int x, int y, int z)
{
    Tile *result = Tile_Copy(current);

    if(Tile_Merge(result, merge, ancestor))
    {
        printf("WARNING: Merge conflict at (%d, %d, %d)!\n", x, y, z);
    }

    return result;
}

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

    int x, y, z;
    Tile *ancestorTile, *currentTile, *mergeTile;
    Tile blankTile = {{NULL, NULL}, {NULL, NULL}, 0, 0, NULL};
    blankTile.area.path = GetPath("/area");
    blankTile.turf.path = GetPath("/turf");

    for(z=0; z<map->levels; z++)
     for(y=0; y<map->height; y++)
      for(x=0; x<map-> width; x++)
    {
        ancestorTile = Map_GetTile(ancestor, x, y, z);
        currentTile = Map_GetTile(current, x, y, z);
        mergeTile = Map_GetTile(merge, x, y, z);

        if(!mergeTile && !currentTile)
        {
            Map_SetTile(map, &blankTile, x, y, z);
        }
        else if(Tile_isEqual(currentTile, mergeTile))
        {
            Map_SetTile(map, currentTile, x, y, z);
        }
        else if(!mergeTile || Tile_isEqual(ancestorTile, mergeTile))
        {
            Map_SetTile(map, currentTile, x, y, z);
        }
        else if(!currentTile || Tile_isEqual(ancestorTile, currentTile))
        {
            Map_SetTile(map, mergeTile, x, y, z);
        }
        else
        {
            Map_SetTile(map, MergeTiles(mergeTile, currentTile, ancestorTile, x, y, z), x, y, z);
        }
    }

    return map;
}

