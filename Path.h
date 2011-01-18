#ifndef MAPMERGE_PATH_H
#define MAPMERGE_PATH_H


typedef struct Path
{
    struct Path *parent;
    unsigned int numChildren, allocatedChildren;
    struct Path **children;
    char name[];
} Path;

Path *GetPath(char *pathText);


#endif
