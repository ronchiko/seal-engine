#pragma once

#include "seal/math/vector2.h"

#include "seal/ecs/component.h"


SEAL_COMPONENT typedef struct Seal_TransformComponent {
	struct Seal_Component base;

	Seal_Vector2 position;
	float angle;
	Seal_Vector2 scale;
} Seal_TransformComponent;

SEAL_COMPONENT_BUFFER_INIT Seal_InitTransformBuffer(void);
SEAL_COMPONENT_BUFFER_GET Seal_GetTransformBuffer(void);

Seal_TransformComponent *Seal_AddTransform(Seal_Entity entity,
	Seal_Vector2 position, float angle, Seal_Vector2 scale);
void Seal_RemoveTransform(Seal_Entity e);