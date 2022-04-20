
#include <math.h>

#include "seal/math.h"

Seal_Vector2 Seal_V2V2Add(Seal_Vector2 a, Seal_Vector2 b) {
	return (Seal_Vector2){ .x = a.x + b.x, .y = a.y + b.y };
}

Seal_Vector2 Seal_V2V2Sub(Seal_Vector2 a, Seal_Vector2 b) {
	return (Seal_Vector2){ .x = a.x - b.x, .y = a.y - b.y };
}

Seal_Vector2 Seal_V2V2Scale(Seal_Vector2 a, Seal_Vector2 b) {
	return (Seal_Vector2){ .x = a.x * b.x, .y = a.y * b.y };
}

Seal_Vector2 Seal_V2Scale(Seal_Vector2 a, float b) {
	return (Seal_Vector2){ a.x * b, a.y * b };
}

Seal_Vector2 Seal_V2Normal(Seal_Vector2 a) {
	float len = Seal_FastInverseSqrt(a.x * a.x + a.y * a.y);
	return Seal_V2Scale(a, len);
}

void Seal_V2Normalize(Seal_Vector2 *a) {
	float len = Seal_FastInverseSqrt(a->x * a->x + a->y * a->y);

	a->x *= len; a->y *= len;
}

Seal_Vector2 Seal_V2V2Lerp(Seal_Vector2 a, Seal_Vector2 b, float t) {
	return (Seal_Vector2){
		.x = Seal_Lerp(a.x, b.x, t),
		.y = Seal_Lerp(a.y, b.y, t)
	};
}

Seal_Float Seal_V2Magnitude(Seal_Vector2 a) {
	return sqrtf(a.x * a.x + a.y * a.y);
}