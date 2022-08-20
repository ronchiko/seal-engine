
#include <seal/testing/suite.h>

#include "ray.h"

int TestRayIntersectsRay_WhenRayValid_ThenReturnsStatistics() {
	int result = 0;
	
	Ray ray = {
		.direction = {
			.x = 1,
			.y = 0
		},
		.origin = {0, 0},
		.maxDistance = 10
	};
	Seal_Rect rect = {
		.x = 3,
		.y = -3,
		.h = 6,
		.w = 6
	};

	RaycastResult raycast_result = {0};
	Seal_Bool hit = RayRectIntersection(&ray, &rect, &raycast_result);

	ASSERT(hit);
	ASSERT(raycast_result.distance == 3);
	ASSERT(raycast_result.normal.x == -1);
	ASSERT(raycast_result.normal.y == 1);
	ASSERT(raycast_result.contact.x == 3);
	ASSERT(raycast_result.contact.y == 0);

cleanup:
	return result;
}

int main() {
	int result = 0;

	RUN_TEST(TestRayIntersectsRay_WhenRayValid_ThenReturnsStatistics);

	return result;
}