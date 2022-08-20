
#include <math.h>

#include <seal/math.h>

#include "ray.h"

#define GetDirectionalNormal(v)		((v) < 0 ? 1 : -1) 

static inline void GetIntersectionPoints(Seal_Vector2 inverse, const Ray *ray, const Seal_Rect *rect,
	Seal_Vector2 *near, Seal_Vector2 *far) {
	
	Seal_Vector2 sizedPosition = Seal_V2V2Add(rect->position, rect->size);

	*near = Seal_V2V2Scale(Seal_V2V2Sub(rect->position, ray->origin), inverse);
	*far = Seal_V2V2Scale(Seal_V2V2Sub(sizedPosition, ray->origin), inverse);
}

static inline Seal_Bool InvalidateIntersectionPoints(Seal_Vector2 *near, Seal_Vector2 *far) {
	if(isnan(far->x) || isnan(far->y) || isnan(near->x) || isnan(near->y)) return SEAL_FALSE;
	
	if (far->x < near->x) Seal_Swapf(&near->x, &far->x);
	if (far->y < near->y) Seal_Swapf(&near->y, &far->y);

	if (near->x > far->y || near->y > far->x) return SEAL_FALSE;

	return SEAL_TRUE;
}

static inline Seal_Vector2 FindNormalOf(Seal_Vector2 near, Seal_Vector2 far, Seal_Vector2 inverse) {
	if (near.x > near.y) return (Seal_Vector2){GetDirectionalNormal(inverse.x), 0};
	if (near.x < near.y) return (Seal_Vector2){0, GetDirectionalNormal(inverse.y)};

	return (Seal_Vector2){0, 0};
}

static inline void GetIntersectionTimes(Seal_Vector2 near, Seal_Vector2 far, float *nearTime, float *farTime) {
	*nearTime = Seal_Maxf(near.x, near.y);
	*farTime = Seal_Minf(far.x, far.y);
}

Seal_Bool RayRectIntersection(const Ray *ray, const Seal_Rect *rect, RaycastResult *result) {	
	if (!ray || !rect) return SEAL_FALSE;

	Seal_Vector2 invserseDirection = { 1.f / ray->direction.x, 1.f / ray->direction.y };
	Seal_Vector2 near, far;

	GetIntersectionPoints(invserseDirection, ray, rect, &near, &far);
	if(InvalidateIntersectionPoints(&near, &far)) return SEAL_FALSE;

	float nearTime, farTime;
	GetIntersectionTimes(near, far, &nearTime, &farTime);

	if(farTime < 0) return SEAL_FALSE;

	if(result) {
		result->distance = Seal_V2Magnitude(ray->direction) * nearTime;
		result->contact = Seal_V2V2Add(ray->origin, Seal_V2Scale(ray->direction, nearTime));
		result->normal = FindNormalOf(near, far, invserseDirection);
	}

	return SEAL_TRUE;
}
