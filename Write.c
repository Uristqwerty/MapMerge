#include <stdio.h>
#include "Map.h"
#include "Write.h"

int WriteMap(Map *output, char *filename)
{
    if(!output)
    {
        printf("Could not write map; null map pointer. Talk to a MapMerge dev about it, this is an internal error.\n");
        return 0;
    }
    else if(!filename)
    {
        printf("Could not write map; null filename. Talk to a MapMerge dev about it, this is an internal error.\n");
        return 0;
    }

    printf("Could not write map; function is unimplemented.\n");
    return 0;
}

