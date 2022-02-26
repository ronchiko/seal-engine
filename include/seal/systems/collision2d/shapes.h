#pragma once

#include <seal/math.h>

typedef enum {
	SEAL_COLLIDER2D_RECT,
	SEAL_COLLIDER2D_GRID
} Seal_CollisionShapeType;

typedef struct {
	Seal_Bool *grid;
	Seal_Size width;
	Seal_Size height;

	Seal_Vector2 cellSize;
} Seal_GridCollisionShape2d;

typedef struct {
	Seal_Vector2 size;
} Seal_RectCollisionShape2d;

struct Seal_Collider2d *Seal_AddRectCollider2d(Seal_Entity en, Seal_Vector2 sizes);
struct Seal_Collider2d *Seal_AddGridCollider2d(Seal_Entity en, Seal_Size w, Seal_Size h);

