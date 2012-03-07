#include <stdlib.h>
#include <string.h>
#include "Path.h"
#include "Vars.h"
#include "TileList.h"
#include "Map.h"

TileList *newTileList()
{
    TileList *new = malloc(sizeof(TileList));
    if(!new)
    {
        printf("Could not create TileList; malloc failed. Talk to a MapMerge dev about it, this is an internal error.\n");
        return NULL;
    }

    new->tiles = calloc(sizeof(TileList), TILELIST_INITIAL_LENGTH);
    if(!new->tiles)
    {
        printf("Could not create TileList; malloc failed. Talk to a MapMerge dev about it, this is an internal error.\n");
        free(new);
        return NULL;
    }

    new->allocatedTiles = TILELIST_INITIAL_LENGTH;
    new->numTiles = 0;
    new->charsPerTile = 0;

    return new;
}

void deleteTileList(TileList *tileList)
{
    if(!tileList)
    {
        printf("Tried to delete null tileList. Talk to a MapMerge dev about it, this is an internal error.\n");
        return;
    }

    free(tileList->tiles);
    free(tileList);
}

unsigned int TileList_AddLine(TileList *this, char *line)
{
    if(!line)
    {
        printf("Could not add line to TileList; null line. Talk to a MapMerge dev about it, this is an internal error.\n");
        return 0;
    }
    if(!this)
    {
        printf("Could not add line to TileList; null TileList. Talk to a MapMerge dev about it, this is an internal error.\n");
        return 0;
    }

    char buffer[8], c = '\0';
    if(sscanf(line, "%7s = %c", buffer, &c) != 2 || c != '(')
    {
        printf("Could not add line to TileList; bad format.\n");
        return 0;
    }

    int len = strlen(buffer);
    if(len < 3)
    {
        printf("Could not add line to TileList; bad format.\n");
        return 0;
    }
    else if(this->charsPerTile == 0)
    {
        this->charsPerTile = len - 2;
    }
    else if(len != this->charsPerTile + 2)
    {
        printf("Could not add line to TileList; bad format.\n");
        return 0;
    }

    int tileNum = 0, i;
    for(i=1; i<len-1; i++)
    {
        if(buffer[i] >= 'a' && buffer[i] <= 'z')
        {
            tileNum = tileNum*52 + buffer[i] - 'a';
        }
        else if(buffer[i] >= 'A' && buffer[i] <= 'Z')
        {
            tileNum = tileNum*52 + buffer[i] - 'A' + 26;
        }
        else
        {
            printf("Could not add line to TileList; bad format.\n");
            return 0;
        }
    }

    if(tileNum != this->numTiles)
    {
        printf("Could not add line to TileList; index out of order.\n");
        return 0;
    }

    Tile *tile = TileList_getNewTile(this);
    if(!tile)
      return 0;

    i = len + 4;
    len = strlen(line);
    int start = i;
    Vars *vars = NULL;
    for(i; i<len; i++)
    {
        if(line[i] == ',' || line[i] == ')')
        {
            char temp[i - start + 1];
            strncpy(temp, &line[start], i - start);
            temp[i - start] = '\0';

            Path *path = GetPath(temp);
            if(!path)
              return 0;

            if(strncmp(temp, "/turf", strlen("/turf")) == 0)
            {
                tile->turf.path = path;
                tile->turf.vars = vars;
            }
            else if(strncmp(temp, "/area", strlen("/area")) == 0)
            {
                tile->area.path = path;
                tile->area.vars = vars;
            }
            else
            {
                MapObject *mapObject = Tile_getNewMapObject(tile);
                if(!mapObject)
                  return 0;

                mapObject->path = path;
                mapObject->vars = vars;
            }
            start = i + 1;
            vars = NULL;
        }
        else if(line[i] == '{')
        {
            vars = Vars_ParseLine(line, &i, len);
            if(vars == NULL)
              return 0;
        }
    }

    return 1;
}

Tile *TileList_getNewTile(TileList *this)
{
    if(!this)
    {
        printf("Could not get a new tile from TileList; null TileList. Talk to a MapMerge dev about it, this is an internal error.\n");
        return NULL;
    }

    if(this->numTiles >= this->allocatedTiles)
    {
        printf("Unimplemented: Expand TileList if it runs out of allocated tiles. Aborting.\n");
        return NULL;
    }

    Tile *tile = &this->tiles[this->numTiles];
    this->numTiles++;

    if(!Tile_Init(tile))
      return NULL;

    return tile;
}

Tile *TileList_GetTile(TileList *this, int index)
{
    if(!this)
    {
        printf("Could not get a tile from TileList; null TileList. Talk to a MapMerge dev about it, this is an internal error.\n");
        return NULL;
    }
    if(index < 0 || index >= this->numTiles)
    {
        printf("Could not get a tile from TileList; index out of range. Talk to a MapMerge dev about it, this is an internal error.\n");
        return NULL;
    }

    return &this->tiles[index];
}

int TileList_IndexOfEqualTile(TileList *this, Tile *search)
{
    if(!this)
    {
        printf("Could not search for a tile in TileList; null TileList. Talk to a MapMerge dev about it, this is an internal error.\n");
        return -1;
    }
    if(!search)
    {
        printf("Could not search for a tile in TileList; null Tile. Talk to a MapMerge dev about it, this is an internal error.\n");
        return -1;
    }

    int i;
    for(i=0; i<this->numTiles; i++)
    {
        if(Tile_isEqual(search, &this->tiles[i]))
          return i;
    }

    return -1;
}

int TileList_AddTile(TileList *this, Tile *add)
{
    if(!this)
    {
        printf("Could not add a tile to TileList; null TileList. Talk to a MapMerge dev about it, this is an internal error.\n");
        return -1;
    }
    if(!add)
    {
        printf("Could not add a tile to TileList; null Tile. Talk to a MapMerge dev about it, this is an internal error.\n");
        return -1;
    }

    Tile *tile = TileList_getNewTile(this);
    if(!tile)
      return -1;

    *tile = *add;
    return this->numTiles - 1;
}


