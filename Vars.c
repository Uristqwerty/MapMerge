#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Vars.h"
#include "MapObject.h"

int Vars_ParseVars(Vars *vars, char *line, int pos, int len);
int Vars_ParseValue(Parameter *parameter, char *line, int pos, int len);

Vars *Vars_ParseLine(char *line, int *position, int length)
{
    if(!line)
    {
        printf("Could not parse vars line; null line. Talk to a MapMerge dev about it, this is an internal error.\n");
        return NULL;
    }
    if(!position)
    {
        printf("Could not parse vars line; null position. Talk to a MapMerge dev about it, this is an internal error.\n");
        return NULL;
    }

    Vars *vars = malloc(sizeof(Vars));
    if(vars == NULL)
    {
        printf("Could not parse vars line; malloc failed.\n");
        return NULL;
    }

    vars->numParameters = 0;
    vars->allocatedParameters = 0;
    vars->parameters = NULL;

    *position = Vars_ParseVars(vars, line, *position, length);
    if(!*position)
      return NULL;

    return vars;
}

Parameter* Vars_newParameter(Vars *vars)
{
    if(vars == NULL)
    {
        printf("Could not add vars parameter; null vars. Talk to a MapMerge dev about it, this is an internal error.\n");
        return NULL;
    }

    if(vars->numParameters >= vars->allocatedParameters)
    {
        vars->allocatedParameters += 4;
        Parameter *temp = realloc(vars->parameters, vars->allocatedParameters * sizeof(Parameter*));
        if(!temp)
        {
            printf("Could not add vars parameter; malloc failure.\n");
            vars->allocatedParameters -= 4;
            return NULL;
        }
        vars->parameters = temp;
    }

    Parameter *new = &vars->parameters[vars->numParameters++];

    return new;
}

int Vars_ParseVars(Vars *vars, char *line, int pos, int len)
{
    if(vars == NULL)
    {
        printf("Could not parse vars; null vars. Talk to a MapMerge dev about it, this is an internal error.\n");
        return 0;
    }
    if(pos < 1)
    {
        printf("Could not parse vars; invalid pos. Talk to a MapMerge dev about it, this is an internal error.\n");
        return 0;
    }
    if(line[pos] != '{')
    {
        printf("Could not parse vars; invalid line. Talk to a MapMerge dev about it, this is an internal error.\n");
        return 0;
    }

    pos++;
    int start = pos;

    for(pos; pos<len; pos++)
    {
        if(line[pos] == ' ')
        {
            Parameter *parameter = Vars_newParameter(vars);
            if(parameter == NULL)
              return 0;

            char *name = calloc(sizeof(char), start - pos + 1);
            if(!name)
            {
                printf("Could not parse vars; malloc failed.\n");
                return 0;
            }

            strncpy(name, &line[start], pos - start);

            parameter->varName = name;

            pos = Vars_ParseValue(parameter, line, pos+1, len);
            if(!pos)
              return 0;
        }
    }

    return pos;
}

int Vars_ParseValue(Parameter *parameter, char *line, int pos, int len)
{
    if(!parameter)
    {
        printf("Could not parse values; null Parameter. Talk to a MapMerge dev about it, this is an internal error.\n");
        return 0;
    }
    if(pos < 1)
    {
        printf("Could not parse values; invalid pos. Talk to a MapMerge dev about it, this is an internal error.\n");
        return 0;
    }
    if(line[pos] != '=' || line[pos + 1] != ' ')
    {
        printf("Could not parse values; invalid line. Talk to a MapMerge dev about it, this is an internal error.\n");
        return 0;
    }

    pos += 2;

    if(line[pos] >= '0' && line[pos] <= '9')
    {
        int i;
        while(line[pos + i] >= '0' && line[pos + i] <= '9')
          i++;

        if(line[pos + i] != ';' && line[pos + i] != '}')
        {
            printf("Could not parse values; invalid number. Talk to a MapMerge dev about it, this is an internal error.\n");
            return 0;
        }
    }
}








