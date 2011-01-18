#include <stdlib.h>
#include <stdio.h>
#include "Map.h"
#include "TileList.h"
#include "Read.h"


int isEmpty(char *line)
{
    if(!line)
      return 1;

    if(line[0] == '\0' || line[0] == '\n')
      return 1;
    else
      return 0;
}


Map *ReadMap(char *filename)
{
    if(!filename)
    {
        printf("Could not read map; null filename. Talk to a MapMerge dev about it, this is an internal error.\n");
        return NULL;
    }

    FILE *mapFile = fopen(filename, "r");
    if(!mapFile)
    {
        printf("Could not read map; invalid filename.\n");
        Usage();
        return NULL;
    }

    TileList *tileList = newTileList();
    if(!tileList)
      return NULL;

    char buffer[65536]; //Arbitrary size, probably excessive, but far better than insufficient. 
    char *line;

    line = fgets(buffer, sizeof(buffer), mapFile);

    while(!isEmpty(line))
    {
        if(!TileList_AddLine(tileList, line))
        {
            deleteTileList(tileList);
            return NULL;
        }

        line = fgets(buffer, sizeof(buffer), mapFile);
    }


    Map *map = newMap();
    if(!map)
      return NULL;

    line = fgets(buffer, sizeof(buffer), mapFile);
    int x, y, z;
    char c;
    while(line)
    {
        if(line[0] == '(')
        {
            if(sscanf(line, "(%d,%d,%d) = {%c", &x, &y, &z, &c) != 4 || c != '"')
            {
                printf("Could not read map; could not read coordinates.\n");
                return NULL;
            }
            if(x != 1 || y != 1 || z != map->levels + 1)
            {
                printf("Could not read map; unexpected coordinates.\n");
                return NULL;
            }
            else
            {
                x = 0;
                y = 0;
                z = map->levels;
                map->levels++;
                map = Map_Resized(map);
                if(!map)
                  return NULL;
            }
        }
        else if(isEmpty(line) || strcmp(line, "\"}\n") == 0 || map->levels == 0)
        {
        }
        else
        {
            int len = strlen(line) - 1;
            int width = len/tileList->charsPerTile;

            if(map->width == 0)
            {
                map->width = width;
                map->height = 0;
            }
            else if(map->width != width)
            {
                printf("Could not read map; width varies.\n");
                return NULL;
            }

            if(map->levels == 1)
            {
                map->height++;
                map = Map_Resized(map);
                if(!map)
                  return NULL;
            }
            else if(y >= map->height)
            {
                printf("Could not read map; height varies.\n");
                return NULL;
            }

            for(x=0; x<width; x++)
            {
                int i, index = 0;
                for(i=0; i<tileList->charsPerTile; i++)
                {
                    char c = line[tileList->charsPerTile * x];
                    if(c >= 'a' && c <= 'z')
                      index = index * 52 + c - 'a';
                    else if(c >= 'A' && c <= 'Z')
                      index = index * 52 + c - 'A' + 26;
                }

                Map_SetTile(map, TileList_GetTile(tileList, index), x, y, z);
            }

            y++;
        }

        line = fgets(buffer, sizeof(buffer), mapFile);
    }

    deleteTileList(tileList);

    fclose(mapFile);

    return map;
}


