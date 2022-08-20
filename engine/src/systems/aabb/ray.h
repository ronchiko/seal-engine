#pragma once

#include <seal/common.h>
#include <seal/math/rect.h>
#include <seal/components/2d/aabb.h>

typedef struct Ray {
	Seal_Vector2 origin;
	Seal_Vector2 direction;
	float maxDistance;
} Ray;

typedef struct {
	float distance;
	Seal_Vector2 contact;
	Seal_Vector2 normal;
} RaycastResult;

Seal_Bool RayRectIntersection(const Ray *ray, const Seal_Rect *rect, RaycastResult *result);

Seal_Bool ForwardCastAABB(const Seal_AABBCollider2d* collider, const Ray* ray, float* distance);