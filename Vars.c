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
        Parameter *temp = realloc(vars->parameters, vars->allocatedParameters * sizeof(Parameter));

        if(temp == NULL)
        {
            printf("Could not create parameter; malloc failure.");
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
        if(line[pos] == ' ' && line[pos + 1] == '=' && line[pos + 2] == ' ')
        {
            Parameter *parameter = Vars_newParameter(vars);
            if(parameter == NULL)
              return 0;

            char *name = calloc(sizeof(char), pos - start + 1);
            if(name == NULL)
            {
                printf("Could not parse vars; malloc failed.\n");
                return 0;
            }

            strncpy(name, &line[start], pos - start);
            parameter->varName = name;

            pos = Vars_ParseValue(parameter, line, pos + 3, len);
            if(!pos)
              return 0;

            if(line[pos] == ';')
            {
                pos += 2;
                start = pos;
            }
            else if(line[pos] == '}')
            {
                break;
            }
        }
    }

    return pos;
}

int Vars_ParseValue(Parameter *parameter, char *line, int pos, int len)
{
    if(!parameter)
    {
        printf("(%s: %d) Could not parse values; null Parameter. Talk to a MapMerge dev about it, this is an internal error.\n", __FILE__, __LINE__);
        return 0;
    }
    if(pos < 1)
    {
        printf("(%s: %d) Could not parse values; invalid pos. Talk to a MapMerge dev about it, this is an internal error.\n", __FILE__, __LINE__);
        return 0;
    }

    parameter->value = calloc(sizeof(Value), 1);
    if(parameter->value == NULL)
    {
        printf("Could not parse value; malloc failed.\n");
        return 0;
    }

    if(line[pos] == ' ')
      pos++;

    if(line[pos] == '-' || (line[pos] >= '0' && line[pos] <= '9'))
    {
        int i = 0;

        if(line[pos] == '-')
          i++;

        while(line[pos + i] >= '0' && line[pos + i] <= '9')
          i++;

        if(line[pos + i] == '.')
        {
            i++;

            while(line[pos + i] >= '0' && line[pos + i] <= '9')
              i++;
        }

        if(line[pos + i] == 'e')
        {
            i++;

            if((line[pos + i] != '-' && line[pos + i] != '+')
              || !(line[pos + i + 1] >= '0' && line[pos + i + 1] <= '9')
              || !(line[pos + i + 2] >= '0' && line[pos + i + 2] <= '9')
              || !(line[pos + i + 3] >= '0' && line[pos + i + 3] <= '9'))
            {
                printf("Could not parse values; invalid number.\n");
                return 0;
            }

            i += 4;
        }

        if(line[pos + i] != ';' && line[pos + i] != '}' && line[pos + i] != ')')
        {
            printf("Could not parse values; invalid number.\n");
            return 0;
        }

        char *num = calloc(sizeof(char), i + 1);

        if(num == NULL)
        {
            printf("Could not parse num value; malloc failed.\n");
            return 0;
        }

        strncpy(num, &line[pos], i);

        parameter->value->type = STRING;
        parameter->value->string = num;
        return pos + i;
    }
    else if(line[pos] == '"')
    {
        int i = 0, escaped = 1;

        while(pos + i < len && (escaped || line[pos + i] != '"'))
        {
            if(escaped)
              escaped = 0;
            else if(line[pos + i] == '\\')
              escaped = 1;
              
            i++;
        }

        i++;

        char *text = calloc(sizeof(char), i + 1);

        if(text == NULL)
        {
            printf("Could not parse text value; malloc failed.\n");
            return 0;
        }

        strncpy(text, &line[pos], i);

        parameter->value->type = STRING;
        parameter->value->string = text;
        return pos + i;
    }
    else if(line[pos] == '\'')
    {
        int i = 0, escaped = 1;

        while(pos + i < len && (escaped || line[pos + i] != '\''))
        {
            if(escaped)
              escaped = 0;
            else if(line[pos + i] == '\\')
              escaped = 1;
              
            i++;
        }

        i++;

        char *text = calloc(sizeof(char), i + 1);

        if(text == NULL)
        {
            printf("Could not parse text value; malloc failed.\n");
            return 0;
        }

        strncpy(text, &line[pos], i);

        parameter->value->type = STRING;
        parameter->value->string = text;
        return pos + i;
    }
    else if(line[pos] == 'n' && line[pos + 1] == 'u' && line[pos + 2] == 'l' && line[pos + 3] == 'l')
    {
        parameter->value->type = STRING;
        parameter->value->string = "null";
        return pos + 4;
    }
    else if((line[pos] >= 'A' && line[pos] <= 'Z') || (line[pos] >= 'a' && line[pos] <= 'z') || line[pos] == '_')
    {
        int i = 0;
        while((line[pos + i] >= 'A' && line[pos + i] <= 'Z') ||
              (line[pos + i] >= 'a' && line[pos + i] <= 'z') ||
              (line[pos + i] >= '0' && line[pos + i] <= '9') || line[pos + i] == '_')
        {
            i++;
        }

        if(line[pos + i] != '(')
        {
            printf("Could not parse function; unexpected %c.\n", line[pos + i]);
            return 0;
        }

        char *name = calloc(sizeof(char), i + 1);
        Vars *vars = malloc(sizeof(Vars));

        if(name == NULL || vars == NULL)
        {
            printf("Could not parse function; malloc failed.\n");
            return 0;
        }

        strncpy(name, &line[pos], i);

        vars->numParameters = 0;
        vars->allocatedParameters = 0;
        vars->parameters = NULL;

        parameter->value->type = FUNCTION;
        parameter->value->function.name = name;
        parameter->value->function.args = vars;

        pos += i + 1;

        while(pos < len && line[pos] != ')')
        {
            Parameter *param = Vars_newParameter(vars);
            if(param == NULL)
              return 0;

            if(line[pos] == ',')
              pos++;

            pos = Vars_ParseValue(param, line, pos, len);
            if(!pos)
              return 0;

            if(line[pos] == ' ' && line[pos + 1] == '=')
            {
                Parameter *assoc = calloc(sizeof(Parameter), 2);
                if(assoc == NULL)
                {
                    printf("Could not parse function parameter associations; malloc failed.\n");
                    return 0;
                }

                assoc[0].value = param->value;
                param->value = calloc(sizeof(Value), 1);
                if(param->value == NULL)
                {
                    printf("Could not parse function parameter associations; malloc failed.\n");
                    return 0;
                }

                param->value->type = ASSOCIATION;
                param->value->association.key = &assoc[0];
                param->value->association.value = &assoc[1];

                pos = Vars_ParseValue(&assoc[1], line, pos + 3, len);
            }
        }

        return pos + 1;
    }

    printf("Unimplemented or invalid var value type.\nLine: %s\nAt: %.10s...", line, &line[pos]);
    return 0;
}

int FuncArgs_isEqual(Vars *this, Vars *other)
{
    if(this->numParameters != other->numParameters)
      return 0;

    int i;
    for(i = 0; i < this->numParameters; i++)
      if(!Parameter_isEqual(this->parameters[i], other->parameters[i]))
        return 0;

    return 1;
}

int Function_isEqual(Value *this, Value *other)
{
    if(strcmp(this->function.name, other->function.name) != 0)
      return 0;

    if(!FuncArgs_isEqual(this->function.args, other->function.args))
      return 0;

    return 1;
}

int Value_isEqual(Value *this, Value *other)
{
    if(this->type != other->type)
      return 0;

    switch(this->type)
    {
        case STRING:
          return strcmp(this->string, other->string) == 0;

        case ASSOCIATION:
          return Parameter_isEqual(this->association.key, other->association.key) &&
                 Parameter_isEqual(this->association.value, other->association.value);

        case FUNCTION:
          return Function_isEqual(this, other);
    }

    return 0;
}

int Parameter_isEqual(Parameter *this, Parameter *other)
{
    if(strcmp(this->varName, other->varName) != 0)
      return 0;

    if(!Value_isEqual(this->value, other->value))
      return 0;

    return 1;
}

int Vars_hasEqual(Vars *this, Parameter *param)
{
    int i;
    for(i = 0; i < this->numParameters; i++)
      if(Parameter_isEqual(&this->parameters[i], param))
        return 1;

    return 0;
}

int Vars_isEqual(Vars *this, Vars *other)
{
    if(this == other)
      return 1;

    if(this == NULL || other == NULL)
      return 0;

    if(this->numParameters != other->numParameters)
      return 0;

    int i;
    for(i = 0; i < this->numParameters; i++)
      if(!Vars_hasEqual(other, &this->parameters[i]))
        return 0;

    return 1;
}

void FuncArgs_Write(Vars *this, FILE *file)
{
    int i;
    for(i = 0; i < this->numParameters; i++)
    {
        if(i > 0)
          fprintf(file, ", ");

        Parameter_Write(&this->parameters[i], file);
    }
}

void Value_Write(Value *this, FILE *file)
{
    switch(this->type)
    {
        case STRING:
          fprintf(file, "%s", this->string);
          break;

        case ASSOCIATION:
          Parameter_Write(this->association.key, file);
          fprintf(file, " = ");
          Parameter_Write(this->association.value, file);
          break;

        case FUNCTION:
          fprintf(file, "%s(", this->function.name);
          FuncArgs_Write(this->function.args, file);
          fprintf(file, ")");
          
    }
}

void Parameter_Write(Parameter *this, FILE *file)
{
    fprintf(file, "%s = ", this->varName);

    Value_Write(this->value, file);
}






