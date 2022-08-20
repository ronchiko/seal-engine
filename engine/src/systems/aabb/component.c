
#include <seal/components/2d/aabb.h>

#include "ray.h"

static Seal_ComponentBuffer gColliderBuffer = SEAL_INVALID_BUFFER;

Seal_ComponentBuffer Seal_GetAABBCollider2dBuffer(void) {
	return gColliderBuffer;
}

void Seal_InitAABBCollider2dBuffer(void) {
	gColliderBuffer = Seal_CreateComponentBuffer(sizeof(Seal_AABBCollider2d));
}

static inline Seal_AABBCollider2d *AddAABBCollider(Seal_Entity entity, Seal_Rect rect) {
	Seal_AABBCollider2d *collider = &(Seal_AABBCollider2d){
		.aabb = rect, 
		.flags = NO_FLAGS,
		.triggerHandler = SEAL_NULL
	};

	Seal_AttachComponent(entity, COMPONENT collider);
	Seal_AABBCollider2d *instance = AABB2D Seal_AddComponent(gColliderBuffer, COMPONENT collider); 
	if (!instance) {
		Seal_LogFatal("AABBCollider2d buffer not initialized");
		return NULL;
	}

	return instance;
}

Seal_AABBCollider2d *Seal_AddInitializedAABBCollider2d(Seal_Entity entity) {
	Seal_Rect aabb = {0, 0, 1, 1};

	Seal_Transform2d *transform = TRANSFORM2D Seal_FindComponentForEntity(Seal_GetTransformBuffer(), entity);
	if (transform) {
		Seal_Vector2 seriesBuffer[4];
		Seal_V2Series series = Seal_V2SeriesFromBuffer(seriesBuffer, 4);
	
		Seal_V2SeriesRotate(series, transform->angle);

		aabb = Seal_V2SeriesRectBounds(series);
	}

	return AddAABBCollider(entity, aabb);	
}

Seal_AABBCollider2d *Seal_AddAABBCollider2d(Seal_Entity entity) {
	return AddAABBCollider(entity, (Seal_Rect){0, 0, 1, 1});
}

void Seal_RemoveAABBCollider2d(Seal_Entity entity) {
	Seal_RemoveComponent(gColliderBuffer, entity);
}

void Seal_MoveAABBCollider2d(Seal_Entity entity, Seal_Vector2 motion) {
	Seal_Transform2d *transform = TRANSFORM2D Seal_FindComponentForEntity(gColliderBuffer, entity);
	if(!transform) {
		Seal_LogWarning("Attempted to move entity without transform (Entity=%zu)", entity);
		return;
	}
	
	Seal_AABBCollider2d *collider = AABB2D Seal_FindComponentForEntity(gColliderBuffer, entity);
	if (!collider) {
		Seal_LogWarning("Attempted to move entity without collider (Entity=%zu)", entity);
		transform->position.x += motion.x;
		transform->position.y += motion.y;
		return;
	}

	Seal_Vector2 direction = Seal_V2Normal(motion); 
	Ray ray = {
		.direction = direction,
		.maxDistance = Seal_V2Magnitude(motion),
		.origin = transform->position
	};

	float moveDistance = 0;
	Seal_Bool collided = ForwardCastAABB(collider, &ray, &moveDistance);
	if(collided) {
		motion = Seal_V2Scale(direction, moveDistance);
	}

	transform->position.x += motion.x;
	transform->position.y += motion.y;		
}