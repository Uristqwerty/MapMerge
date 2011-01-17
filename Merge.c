#include <stdio.h>
#include "Map.h"
#include "Merge.h"

Map *MergeMaps(Map *ancestor, Map *current, Map *merge)
{
    if(!ancestor || !current || !merge)
    {
        printf("Could not merge maps; at least one of the passed Map pointers was null. Talk to a dev about it, this is an internal error.\n");
        return 0;
    }

    printf("Could not merge maps; function is unimplemented.\n");
    return 0;
}

