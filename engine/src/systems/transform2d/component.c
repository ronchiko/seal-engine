
#include "seal/ecs/component.h"
#include "seal/components/2d/transform.h"

static Seal_ComponentBuffer gTransformBuffer = SEAL_INVALID_BUFFER;

Seal_ComponentBuffer Seal_GetTransformBuffer(void) {
	return gTransformBuffer;
}

void Seal_InitTransformBuffer(void) {
	gTransformBuffer = Seal_CreateComponentBuffer(sizeof(Seal_Transform2d));
}

Seal_Transform2d *Seal_AddTransform(Seal_Entity entity,
	Seal_Vector2 position, float angle, Seal_Vector2 scale) {
	
	Seal_Transform2d transform = {
		.base = { 0, 0 },
		.angle = angle,
		.position = { position.x, position.y },
		.scale = { scale.x, scale.y }
	};

	Seal_AttachComponent(entity, COMPONENT &transform);
	Seal_Transform2d *t2d = TRANSFORM2D Seal_AddComponent(gTransformBuffer, COMPONENT &transform);
	if (!t2d) {
		Seal_LogFatal("Transform2d buffer is not initialized");
		return NULL;
	}

	return t2d;
}

void Seal_RemoveTransform(Seal_Entity e) {
	Seal_RemoveComponent(gTransformBuffer, e);
}