
#include "seal/ecs/component.h"
#include "seal/components/2d/transform.h"

static Seal_ComponentBuffer gTransformBuffer;

Seal_ComponentBuffer Seal_GetTransformBuffer(void) {
	return gTransformBuffer;
}

void Seal_InitTransformBuffer(void) {
	gTransformBuffer = Seal_CreateComponentBuffer(sizeof(Seal_TransformComponent));
}

Seal_TransformComponent *Seal_AddTransform(Seal_Entity entity,
	Seal_Vector2 position, float angle, Seal_Vector2 scale) {
	
	Seal_TransformComponent transform = {
		.base = { 0, 0 },
		.angle = angle,
		.position = { position.x, position.y },
		.scale = { scale.x, scale.y }
	};

	Seal_AttachComponent(entity, &transform.base);
	return (Seal_TransformComponent *)Seal_AddComponent(gTransformBuffer, &transform.base);
}

void Seal_RemoveTransform(Seal_Entity e) {
	Seal_RemoveComponent(gTransformBuffer, e);
}