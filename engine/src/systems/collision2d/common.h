#pragma once

#include <seal.h>

typedef struct {
	Seal_Collider2d collider;
	Seal_Rect aabb;
} Seal_RawCollider2d;

struct Seal_ShapeMethodVTable {
	void (*setAABB)(Seal_RawCollider2d *);
};

extern struct Seal_ShapeMethodVTable gShapeMethodTable[];

#define Seal_SetAABB(shape, collider) (*gShapeMethodTable[(shape)].setAABB)(collider)

void RectSetAABB(Seal_RawCollider2d *c);