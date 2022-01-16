
#include <math.h>

#include "seal/math.h"

#include "gmatrix.h"


void Seal_M2Identity(Seal_Matrix2x2 matrix) {
	_Seal_MatrixIdentity(matrix, 2);
}

void Seal_M2Screen(Seal_Matrix2x2 matrix, int ww, float ratio, float zoom) {
	matrix[1] = matrix[2] = 0;
	matrix[3] = ratio * (matrix[0] = zoom / ww);
}

void Seal_M2Rotation(Seal_Matrix2x2 matrix, float angle) {
	matrix[2] = -(matrix[1] = sinf(angle));
	matrix[0] = matrix[3] = cosf(angle);
}

void Seal_M2Transpose(Seal_Matrix2x2 matrix) {
	_Seal_MatrixTranspose(matrix, matrix, 2);
}