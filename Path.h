#ifndef MAPMERGE_PATH_H
#define MAPMERGE_PATH_H

#include <stdio.h>


typedef struct Path
{
    struct Path *parent;
    unsigned int numChildren, allocatedChildren;
    struct Path **children;
    char name[];
} Path;

Path *GetPath(char *pathText);

void Path_Write(Path *this, FILE *out);

#endif
