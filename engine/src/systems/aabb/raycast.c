#include "ray.h"

static Seal_Bool RaycastAgainstEntity(
	const Seal_AABBCollider2d *collider,
	const Seal_Transform2d *transform,
	const Ray *ray,
	RaycastResult* results
) {
	Seal_Rect aabb = {
		.x = transform->position.x + collider->aabb.x * transform->scale.x,
		.y = transform->position.y + collider->aabb.y * transform->scale.y,
		.w = collider->aabb.w * transform->scale.x,
		.h = collider->aabb.h * transform->scale.y
	};

	return RayRectIntersection(ray, &aabb, results);
}

Seal_Bool ForwardCastAABB(const Seal_AABBCollider2d *collider, const Ray *ray, Seal_Float *distance) {
	*distance = ray->maxDistance;
	Seal_Bool hit = SEAL_FALSE;

	RaycastResult result = {0};
	Seal_AABBCollider2d *other_collider = NULL;
	for(Seal_Int i = 0; Seal_GetComponent(Seal_GetAABBCollider2dBuffer(), ++i, &COMPONENT other_collider); ++i) {
		if(other_collider->base.componentId == collider->base.componentId) {
			continue;
		}

		Seal_Transform2d *transform = TRANSFORM2D Seal_FindComponentForEntity(
			Seal_GetTransformBuffer(), 
			other_collider->base.parentId);
		if(!transform) {
			Seal_LogWarning("Entity has collider but no transform (Entity=%zu)", other_collider->base.parentId);
			continue;
		}

		if(RaycastAgainstEntity(other_collider, transform, ray, &result)) {
			if(result.distance <= *distance) {
				*distance = result.distance;
			}

			Seal_LogInfo("Entity was hit (%zu) - (%f)", other_collider->base.parentId, *distance);
			hit = SEAL_TRUE;
		}
	}	

	return hit;
}