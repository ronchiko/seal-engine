#pragma once

#include "seal/common.h"
#include "seal/math/vector2.h"
#include "seal/math/matrix2.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef float Seal_Matrix3x3[9];

void Seal_M3Identity	(Seal_Matrix3x3 matrix);
void Seal_M3Rotation	(Seal_Matrix3x3 matrix, float angle);
void Seal_M3Translate	(Seal_Matrix3x3 matrix, float x, float y);
void Seal_M3Scale		(Seal_Matrix3x3 matrix, float x, float y);

void Seal_M3Transpose	(Seal_Matrix3x3 matrix);
void Seal_M3Multiply	(Seal_Matrix3x3 out, Seal_Matrix3x3 a, Seal_Matrix3x3 b);
void Seal_M3Transform	(Seal_Matrix3x3 out, Seal_Vector2 position, Seal_Vector2 scale, float angle);

void Seal_M3Camera		(Seal_Matrix3x3 out, Seal_Camera camera);

#ifdef __cplusplus
}
#endif
