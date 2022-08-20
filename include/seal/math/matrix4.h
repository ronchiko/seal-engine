#pragma once

#include "seal/math/matrix3.h"
#include "seal/math/rect.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef float Seal_Matrix4x4[16];

void Seal_M4Identity	(Seal_Matrix4x4 matrix);
void Seal_M4Ortho		(Seal_Matrix4x4 matrix, Seal_Rect projection_rect, Seal_Vector2 farnear);
void Seal_M4Transpose	(Seal_Matrix4x4 matrix);

#ifdef __cplusplus
}
#endif
