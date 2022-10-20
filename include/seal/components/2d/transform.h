#pragma once

#include "seal/math/vector2.h"

#include "seal/ecs/component.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TRANSFORM2D 	(Seal_Transform2d *)

typedef struct Seal_Transform2d {
	struct Seal_Component base;

	Seal_Vector2 position;
	float angle;
	Seal_Vector2 scale;
} Seal_Transform2d;

void Seal_InitTransformBuffer(Seal_ComponentBuffer);
Seal_ComponentBuffer Seal_GetTransformBuffer(void);

Seal_Transform2d *Seal_AddTransform(Seal_Entity entity,
	Seal_Vector2 position, float angle, Seal_Vector2 scale);
void Seal_RemoveTransform(Seal_Entity e);

#ifdef __cplusplus
}
#endif