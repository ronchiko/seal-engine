
#include "common.h"

struct Seal_ShapeMethodVTable gShapeMethodTable[] = {
	[SEAL_SHAPE2D_RECT] = { &RectSetAABB }
};