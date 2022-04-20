#pragma once

#include "seal/common.h"

typedef union Seal_Vector2 {
	struct {
		Seal_Float x;
		Seal_Float y;
	};
	Seal_Float array[2];
} Seal_Vector2;

Seal_Vector2 Seal_V2V2Add(Seal_Vector2 a, Seal_Vector2 b);
Seal_Vector2 Seal_V2V2Sub(Seal_Vector2 a, Seal_Vector2 b);
Seal_Vector2 Seal_V2Scale(Seal_Vector2 a, float scale);
Seal_Vector2 Seal_V2V2Scale(Seal_Vector2 a, Seal_Vector2 b);

Seal_Vector2 Seal_V2Normal(Seal_Vector2 a);
Seal_Float Seal_V2Magnitude(Seal_Vector2 a);
Seal_Vector2 Seal_V2V2Lerp(Seal_Vector2 a, Seal_Vector2 b, float t);

void Seal_V2Normalize(Seal_Vector2 *a);