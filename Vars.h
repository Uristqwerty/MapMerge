#ifndef MAPMERGE_VARS_H
#define MAPMERGE_VARS_H


struct MapObject;

typedef struct Value
{
    enum
    {
        FUNCTION,
        STRING,
        MAPOBJECT
    } type;

    union
    {
        struct
        {
            char *name;
            struct Vars *args;
        } function;

        char *string;

        struct MapObject *mapobject;
    };
} Value;

typedef struct Parameter
{
    char *varName;
    Value *value;
} Parameter;

typedef struct Vars
{
    int numParameters, allocatedParameters;
    Parameter *parameters;
} Vars;

Vars *Vars_ParseLine(char *line, int *position);


#endif
