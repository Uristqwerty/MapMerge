#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "MapObject.h"
#include "Tile.h"

MapObject *Tile_getNewMapObject(Tile *this)
{
    if(!this)
    {
        printf("Could not get new MapObject from Tile; null Tile. Talk to a MapMerge dev about it, this is an internal error.\n");
        return NULL;
    }

    if(this->numObjects >= this->allocatedObjects)
    {
        MapObject *objects = realloc(this->objects, sizeof(MapObject) * (this->allocatedObjects + 8));
        if(!objects)
        {
            printf("Could not get new MapObject from Tile; malloc failure. Talk to a MapMerge dev about it, this is an internal error.\n");
            return NULL;
        }

        this->objects = objects;
        this->allocatedObjects += 8;
    }

    MapObject *mapObject = &this->objects[this->numObjects];
    this->numObjects++;

    mapObject->path = NULL;
    mapObject->vars = NULL;

    return mapObject;
}

int Tile_areContentsEqual(Tile *this, Tile *other)
{
    if(this->numObjects != other->numObjects)
      return 0;

    int i;
    for(i=0; i<this->numObjects; i++)
      if(!MapObject_isEqual(&this->objects[i], &other->objects[i]))
        return 0;

    return 1;
}

int Tile_isEqual(Tile *this, Tile *other)
{

    if(!this || !other)
      return 0;

    if(!MapObject_isEqual(&this->area, &other->area))
      return 0;

    if(!MapObject_isEqual(&this->turf, &other->turf))
      return 0;

    if(!Tile_areContentsEqual(this, other))
      return 0;

    return 1;
}

void Tile_Write(Tile *this, FILE *file)
{
    if(!this)
    {
        printf("Could not write Tile; null Tile. Talk to a MapMerge dev about it, this is an internal error.\n");
        return;
    }
    if(!file)
    {
        printf("Could not write Tile; null file. Talk to a MapMerge dev about it, this is an internal error.\n");
        return;
    }

    int i;
    for(i=0; i<this->numObjects; i++)
    {
        MapObject_Write(&this->objects[i], file);
        fprintf(file, ",");
    }

    MapObject_Write(&this->turf, file);
    fprintf(file, ",");
    MapObject_Write(&this->area, file);
}

int Tile_Init(Tile *this)
{
    if(!this)
    {
        printf("Could not init Tile; null Tile. Talk to a MapMerge dev about it, this is an internal error.\n");
        return 0;
    }

    this->area.path = NULL;
    this->area.vars = NULL;
    this->turf.path = NULL;
    this->turf.vars = NULL;

    this->objects = NULL;
    this->numObjects = 0;
    this->allocatedObjects = 0;

    return 1;
}

Tile* Tile_Copy(Tile *this)
{
    if(this == NULL)
      return NULL;

    Tile *result = malloc(sizeof(Tile));

    if(result == NULL)
      return NULL;

    result->area.path = this->area.path;
    result->area.vars = Vars_Copy(this->area.vars);
    result->turf.path = this->turf.path;
    result->turf.vars = Vars_Copy(this->turf.vars);

    result->numObjects = this->numObjects;
    result->allocatedObjects = this->allocatedObjects;
    result->objects = malloc(sizeof(MapObject) * this->allocatedObjects);

    int i;
    for(i = 0; i < this->numObjects; i++)
    {
        result->objects[i].path = this->objects[i].path;
        result->objects[i].vars = Vars_Copy(this->objects[i].vars);
    }

    return result;
}

void Tile_AddError(Tile *this, char *errstr)
{
    MapObject *errObj = Tile_getNewMapObject(this);
    if(errObj == NULL)
    {
        printf("Could not create a merge error object, unable to allocate a new object.\n");
        return;
    }

    errObj->path = GetPath("/obj/merge_error");
    errObj->vars = malloc(sizeof(Vars));

    if(errObj->vars == NULL)
    {
        printf("Could not create a merge error object, unable to allocate vars.\n");
        return;
    }

    errObj->vars->parameters = malloc(sizeof(Parameter));

    if(errObj->vars->parameters == NULL)
    {
        errObj->vars->numParameters = 0;
        errObj->vars->allocatedParameters = 0;
        printf("Could not create a merge error object, unable to allocate parameters.\n");
        return;
    }

    errObj->vars->numParameters = 1;
    errObj->vars->allocatedParameters = 1;

    Parameter *param = &errObj->vars->parameters[0];
    param->varName = "desc";

    param->value = malloc(sizeof(Value));
    if(param->value == NULL)
    {
        printf("Could not create a merge error object, unable to allocate value.\n");
        return;
    }

    param->value->type = STRING;
    param->value->string = errstr;
}

char* Tile_MergeContents(Tile *this, Tile *other, Tile *ancestor)
{
    return "Merging tile contents not implemented yet.";
}

int Tile_Merge(Tile *this, Tile *other, Tile *ancestor)
{
    if(this->area.path != other->area.path || this->turf.path != other->turf.path)
    {
        Tile_AddError(this, "\"Both the current and merging maps have altered this tile differently, and, since the /turf and/or /area does not match, a merge was not attempted.\"");
        return 1;
    }

    if(Vars_isEqual(this->area.vars, other->area.vars) && !!Vars_isEqual(other->area.vars, ancestor->area.vars))
    {
        if(Vars_isEqual(this->area.vars, ancestor->area.vars))
        {
            this->area.vars = other->area.vars;
        }
        else
        {
            char *errstr = Vars_Merge(this->area.vars, other->area.vars, ancestor->area.vars);

            if(errstr != NULL)
            {
                char buffer[strlen(errstr) + strlen("\"Error merging area vars: \"") + 1];
                sprintf(buffer, "\"Error merging area vars: %s\"", errstr);
                Tile_AddError(this, buffer);
                return 1;
            }
        }
    }

    if(Vars_isEqual(this->turf.vars, other->turf.vars) && !Vars_isEqual(other->turf.vars, ancestor->turf.vars))
    {
        if(Vars_isEqual(this->turf.vars, ancestor->turf.vars))
        {
            this->turf.vars = other->turf.vars;
        }
        else
        {
            char *errstr = Vars_Merge(this->turf.vars, other->turf.vars, ancestor->turf.vars);

            if(errstr != NULL)
            {
                char buffer[strlen(errstr) + strlen("\"Error merging turf vars: \"") + 1];
                sprintf(buffer, "\"Error merging turf vars: %s\"", errstr);
                Tile_AddError(this, buffer);
                return 1;
            }
        }
    }

    if(!Tile_areContentsEqual(this, other) && !Tile_areContentsEqual(other, ancestor))
    {
        if(Tile_areContentsEqual(this, ancestor))
        {
            this->numObjects = other->numObjects;
            this->allocatedObjects = other->allocatedObjects;
            this->objects = other->objects;
        }
        else
        {
            char *errstr = Tile_MergeContents(this, other, ancestor);

            if(errstr != NULL)
            {
                Tile_AddError(this, errstr);
                return 1;
            }
        }
    }

    return 0;
}



