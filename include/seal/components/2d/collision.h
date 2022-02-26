#pragma once

#include <seal/math.h>
#include <seal/ecs/component.h>

#include <seal/systems/collision2d/shapes.h>

typedef struct Seal_Collider2d {
	Seal_Component base;

	Seal_ID transformComponent;
	int32_t collisionLayer;

	Seal_Vector2 aabb;
	const Seal_CollisionShapeType shapeType;
	union {
		Seal_GridCollisionShape2d grid;
	} shape;
} Seal_Collider2d;

void Seal_InitializeCollision2d(void);

void Seal_ColliderMove(Seal_Collider2d *collider);