#pragma once

#include "seal/common.h"
#include "seal/math/vector2.h"

typedef union Seal_Rect {
	Seal_Float pointer[4];
	struct {
		Seal_Float x;
		Seal_Float y;
		Seal_Float h;
		Seal_Float w;
	};
	struct {
		Seal_Vector2 position;
		Seal_Vector2 size;
	};
} Seal_Rect;