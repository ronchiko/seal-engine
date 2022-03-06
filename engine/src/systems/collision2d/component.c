
#include <seal.h>

#include "common.h"

static Seal_ComponentBuffer gColliderBuffers;

void Seal_InitCollision2dBuffer(void) {
	gColliderBuffers = Seal_CreateComponentBuffer(sizeof(Seal_RawCollider2d));
}

Seal_ComponentBuffer Seal_GetCollision2dBuffer(void) {
	return gColliderBuffers;
}

Seal_Collider2d *Seal_AddCollider2d(Seal_Entity entity, Seal_Shape2dEnum shape) {
	Seal_RawCollider2d cloneCollider = (Seal_RawCollider2d){
		.collider.isSolid = SEAL_TRUE,
		.collider.layer = Seal_Bit(1),
		.collider.shape = shape,
		.aabb = { 0 },
		.collider.rect = { 0 }
	};
	Seal_AttachComponent(entity, COMPONENT &cloneCollider);
	
	Seal_RawCollider2d *collider = (Seal_RawCollider2d *)Seal_AddComponent(gColliderBuffers, COMPONENT &cloneCollider);
	
	Seal_SetAABB(shape, collider);
	return &collider->collider;
}

void Seal_RemoveCollider2d(Seal_Entity entity) {
	Seal_RemoveComponent(gColliderBuffers, entity);
}