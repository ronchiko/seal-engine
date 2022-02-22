#pragma once

typedef float Seal_Matrix2x2[4];

void Seal_M2Identity	(Seal_Matrix2x2 matrix);
void Seal_M2Rotation	(Seal_Matrix2x2 matrix, float angle);
void Seal_M2Transpose	(Seal_Matrix2x2 matrix);

void Seal_M2Screen(Seal_Matrix2x2 matrix, float screenWidth, float ratio, float zoom);
