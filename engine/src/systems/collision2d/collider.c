
#include <seal.h>
#include <seal/components/2d/collision.h>

static Seal_ComponentBuffer gColliderBuffers;

void Seal_InitializeCollider2d(void) {
	gColliderBuffers = Seal_CreateComponentBuffer(sizeof(Seal_Collider2d));
}

Seal_Collider2d *Seal_AddRectCollider2d(Seal_Entity entity, Seal_Vector2 v2) {
	Seal_Collider2d collider = {
		.shapeType = SEAL_COLLIDER2D_RECT,
		.aabb = { v2.x, v2.y },
		.collisionLayer = 1,
		.base = { 0, 0 },
	};

	Seal_AttachComponent(entity, &collider);

	return (Seal_Collider2d *)Seal_AddComponent(gColliderBuffers, &collider);
}