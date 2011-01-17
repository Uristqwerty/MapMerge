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
        printf("Could not read map; null filename. Talk to a dev about it, this is an internal error.\n");
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
        
        line = fgets(buffer, sizeof(buffer), mapFile);
    }

    printf("Could not read map; function is unimplemented.\n");
    return NULL;
}

