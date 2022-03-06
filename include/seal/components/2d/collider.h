#pragma once

#include <seal.h>

#include <seal/systems/collision2d/shapes.h>
#include <seal/systems/collision2d/raycast.h>

typedef struct {
	Seal_Component base;

	const Seal_Shape2dEnum shape;
	union {
		Seal_RectShape2d rect;
	};
	Seal_Bool isSolid;
	Seal_BitMask layer;
} Seal_Collider2d;

Seal_Collider2d *Seal_AddCollider2d(Seal_Entity entity, Seal_Shape2dEnum shape);
void Seal_RemoveCollider2d(Seal_Entity entity);

void Seal_InitCollision2dBuffer(void);
Seal_ComponentBuffer Seal_GetCollision2dBuffer(void);


