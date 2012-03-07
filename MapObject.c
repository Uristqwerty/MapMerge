#include <stdio.h>
#include "Path.h"
#include "MapObject.h"


void MapObject_Write(MapObject *this, FILE *file)
{
    if(!this)
    {
        printf("Could not write MapObject; null MapObject. Talk to a MapMerge dev about it, this is an internal error.\n");
        return;
    }
    if(!file)
    {
        printf("Could not write MapObject; null file. Talk to a MapMerge dev about it, this is an internal error.\n");
        return;
    }

    Path_Write(this->path, file);

    if(this->vars)
    {
        fprintf(file, "{");

        int i;
        for(i = 0; i < this->vars->numParameters; i++)
        {
            if(i > 0)
              fprintf(file, "; ");

            Parameter_Write(&this->vars->parameters[i], file);
        }

        fprintf(file, "}");
    }
}

int MapObject_isEqual(MapObject *this, MapObject *other)
{
    if(!this || !other)
      return 0;

    if(this->path != other->path)
      return 0;

    if(!Vars_isEqual(this->vars, other->vars))
      return 0;

    return 1;
}
