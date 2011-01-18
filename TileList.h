#ifndef MAPMERGE_TILELIST_H
#define MAPMERGE_TILELIST_H

#include "map.h"

#define TILELIST_INITIAL_LENGTH 1024


typedef struct TileList
{
    unsigned int numTiles, allocatedTiles, charsPerTile;
    Tile *tiles;
} TileList;


void deleteTileList(TileList *tileList);

Tile *TileList_getNewTile(TileList *this);

TileList *newTileList();

unsigned int TileList_AddLine(TileList *this, char *line);

#endif
