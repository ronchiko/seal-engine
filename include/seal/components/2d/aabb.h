#pragma once

#include <seal.h>


typedef struct {
	Seal_UByte flags;
	Seal_Rect aabb;
} Seal_AABB;

#define SEAL_MAX_SPRITES_PER_GRID		SEAL_UBYTE_MAX - 1

typedef enum {
	SEAL_GRID_INVISIBLE = Seal_Bit(0),
	SEAL_GRID_SOLID = Seal_Bit(1)
} Seal_GridFlags;

typedef struct {
	Seal_UByte flags;
	Seal_TextureId texture;
	Seal_Rect portion;
} Seal_GridSprite;


#define SEAL_GRID_AIR			0


typedef struct {
	Seal_UByte flags;

	Seal_UByte *cells;					// cell[x + y * w] == 0 is an empty cell
	Seal_Size width, height;

	Seal_GridSprite *spritesIndexer;
} Seal_Grid;

Seal_Grid *Seal_CreateGrid(Seal_Size w, Seal_Size h, Seal_UByte flags);
void Seal_FreeGrid(Seal_Grid *grid);

Seal_UByte Seal_GridRegisterTile(Seal_Grid *grid, Seal_GridSprite data);

void Seal_GridAddTile(Seal_Grid *grid, Seal_Int2 position, Seal_UByte tileType);
void Seal_GridAddTileRange(Seal_Grid *grid, Seal_Int2 postion, Seal_Int2 size, Seal_UByte tileType);

