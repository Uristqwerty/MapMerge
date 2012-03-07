#include <stdio.h>
#include "Tile.h"
#include "Map.h"
#include "TileList.h"
#include "Write.h"

void WriteIndex(int index, int digits, FILE *out)
{
    char c[digits], d = digits;
    while(d)
    {
        int t = index % 52;
        if(t >= 26)
          c[d-1] = 'A' + t - 26;
        else
          c[d-1] = 'a' + t;
        index /= 52;
        d--;
    }
    for(d=0; d<digits; d++)
      fprintf(out, "%c", c[d]);
}

int WriteMap(Map *map, char *filename)
{
    if(!map)
    {
        printf("Could not write map; null map pointer. Talk to a MapMerge dev about it, this is an internal error.\n");
        return 0;
    }
    else if(!filename)
    {
        printf("Could not write map; null filename. Talk to a MapMerge dev about it, this is an internal error.\n");
        return 0;
    }

    FILE *mapFile = fopen(filename, "w");
    if(!mapFile)
    {
        printf("Could not write map; unable to open output file for writing.\n");
        return 0;
    }

    TileList *tileList = newTileList();

    int x, y, z, index;
    Tile *tile;
    int data[map->width][map->height][map->levels];

    for(z=0; z<map->levels; z++)
     for(y=0; y<map->height; y++)
      for(x=0; x<map-> width; x++)
    {
        tile = Map_GetTile(map, x, y, z);

        index = TileList_IndexOfEqualTile(tileList, tile);
        if(index == -1)
          index = TileList_AddTile(tileList, tile);

        data[x][y][z] = index;
    }

    index = tileList->numTiles - 1;
    int i = 0;
    while(index)
    {
        i++;
        index /= 52;
    }

    for(index=0; index<tileList->numTiles; index++)
    {
        fprintf(mapFile, "\"");
        WriteIndex(index, i, mapFile);
        fprintf(mapFile, "\" = (");
        Tile_Write(TileList_GetTile(tileList, index), mapFile);
        fprintf(mapFile, ")\n");
    }

    for(z=0; z<map->levels; z++)
    {
        fprintf(mapFile, "\n(1,1,%d) = {\"\n", z + 1);
        for(y=0; y<map->height; y++)
        {
            for(x=0; x<map->width; x++)
              WriteIndex(data[x][y][z], i, mapFile);
            fprintf(mapFile, "\n");
        }
        fprintf(mapFile, "\"}\n");
    }

    fflush(mapFile);
    fclose(mapFile);
    return 1;
}

