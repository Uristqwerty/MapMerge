#ifndef MAPMERGE_VARS_H
#define MAPMERGE_VARS_H


struct MapObject;
struct Parameter;

typedef struct Value
{
    enum
    {
        FUNCTION,
        STRING,
        MAPOBJECT,
        ASSOCIATION
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

        struct
        {
            struct Parameter *key, *value;
        } association;
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

Vars* Vars_ParseLine(char *line, int *position, int length);
Vars* Vars_Copy(Vars *this);

int Vars_isEqual(Vars *this, Vars *other);

void Parameter_Write(Parameter *this, FILE *file);

char* Vars_Merge(Vars *this, Vars *other, Vars *ancestor);

#endif
