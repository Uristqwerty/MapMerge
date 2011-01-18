#include <stdlib.h>
#include <stdio.h>
#include "Path.h"


Path *Path_GetChild(Path *this, char *pathText)
{
    if(!this)
    {
        printf("Could not retrieve Path child; null Path. Talk to a MapMerge dev about it, this is an internal error.\n");
        return NULL;
    }
    if(!pathText)
    {
        printf("Could not retrieve Path child; null pathText. Talk to a MapMerge dev about it, this is an internal error.\n");
        return NULL;
    }

    int i;
    for(i=0; i<this->numChildren; i++)
    {
        if(strcmp(this->children[i]->name, pathText) == 0)
          return this->children[i];
    }

    if(this->numChildren == this->allocatedChildren)
    {
        Path **newChildren = realloc(this->children, sizeof(Path*) * (this->allocatedChildren + 16));
        if(!newChildren)
        {
            printf("Could not retrieve Path child; failed to reallocate Children. Talk to a MapMerge dev about it, this is an internal error.\n");
            return NULL;
        }
        this->children = newChildren;
        this->allocatedChildren += 16;
    }

    this->children[this->numChildren] = malloc(sizeof(Path) + strlen(pathText) + 1);
    if(!this->children[this->numChildren])
    {
        printf("Could not retrieve Path child; failed to allocate new Path. Talk to a MapMerge dev about it, this is an internal error.\n");
        return NULL;
    }

    this->children[this->numChildren]->parent = this;
    this->children[this->numChildren]->numChildren = 0;
    this->children[this->numChildren]->allocatedChildren = 0;
    this->children[this->numChildren]->children = NULL;

    strcpy(this->children[this->numChildren]->name, pathText);

    printf("Added new Path, \"%s\". Parent: \"%s\"\n", this->children[this->numChildren]->name, this->name);

    this->numChildren++;
    return this->children[this->numChildren - 1];
}


Path rootPath = {NULL, 0, 0, NULL, ""};

Path *GetPath(char *pathText)
{
    if(!pathText)
    {
        printf("Could not retrieve Path; null pathText. Talk to a MapMerge dev about it, this is an internal error.\n");
        return NULL;
    }
    if(pathText[0] != '/')
    {
        printf("Could not retrieve Path; invalid pathText. Talk to a MapMerge dev about it, this is an internal error.\n");
        return NULL;
    }

    Path *path = &rootPath;
    int i, start = 1, len = strlen(pathText);

    for(i=1; i<=len; i++)
    {
        if(pathText[i] == '/' || i == len)
        {
            char temp[i - start];
            strncpy(temp, &pathText[start], i - start);
            temp[i - start] = '\0';

            path = Path_GetChild(path, temp);
            if(!path)
              return NULL;
            start = i + 1;
        }
    }

    return path;
}

void Path_Write(Path *this, FILE *out)
{
    if(!this)
    {
        printf("Could not write Path; null Path. Talk to a MapMerge dev about it, this is an internal error.\n");
        return;
    }

    if(this->parent)
    {
        Path_Write(this->parent, out);
        fprintf(out, "/");
    }

    fprintf(out, "%s", this->name);
}



