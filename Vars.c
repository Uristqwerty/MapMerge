#include <stdlib.h>
#include <stdio.h>
#include "Vars.h"
#include "MapObject.h"

Vars *Vars_ParseLine(char *line, int *position)
{
    if(!line)
    {
        printf("Could not parse vars; null line. Talk to a MapMerge dev about it, this is an internal error.\n");
        return NULL;
    }
    if(!position)
    {
        printf("Could not parse vars; null position. Talk to a MapMerge dev about it, this is an internal error.\n");
        return NULL;
    }

    Vars *vars = malloc(sizeof(Vars));
    if(!vars)
    {
        printf("Could not parse vars; malloc failed.\n");
        return NULL;
    }

    vars->numParameters = 0;
    vars->allocatedParameters = 0;
    vars->parameters = NULL;


    return vars;
}
