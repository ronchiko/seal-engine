#pragma once

#include <seal/math/rect.h>

typedef enum {
	SEAL_SHAPE2D_RECT	= 1
} Seal_Shape2dEnum;

typedef struct {
	Seal_Rect rect;
} Seal_RectShape2d;