
#include "seal/ecs/component.h"

static Seal_Entity gLastEntity = 0;

Seal_Entity Seal_CreateEntity(void) {
	return ++gLastEntity;
}

void Seal_DestroyEntity(Seal_Entity id) {
	for(Seal_Size i = 0; i < Seal_CountComponentBuffers(); ++i) {
		Seal_RemoveComponent((Seal_ComponentBuffer)i, id);
	}
}

void Seal_PurgeEntity(Seal_Entity id) {
	for(Seal_Size i = 0; i < Seal_CountComponentBuffers(); ++i) {
		Seal_RemoveComponent((Seal_ComponentBuffer)i, id);
		Seal_TrimComponentBuffer((Seal_ComponentBuffer)i);
	}
}

Seal_ID Seal_AttachComponent(Seal_Entity entity, Seal_Component *component) {
	static Seal_ID gLastComponentId = 0;
	if (entity < 0 || entity > gLastEntity) {
		Seal_LogError("Invalid entity %zu", entity);
		return -1;
	}

	component->componentId = ++gLastComponentId;
	component->parentId = entity;

	return component->componentId;
}

