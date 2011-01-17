#include <stdio.h>
#include "Map.h"
#include "Read.h"

Map *ReadMap(char *filename)
{
    if(!filename)
    {
        printf("Could not read map; null filename. Talk to a dev about it, this is an internal error.\n");
        return 0;
    }

    printf("Could not read map; function is unimplemented.\n");
    return 0;
}

