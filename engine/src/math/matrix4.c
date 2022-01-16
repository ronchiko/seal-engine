
#include <math.h>

#include "seal/math.h"

#include "gmatrix.h"

extern int Seal_Min(int a, int b);
extern int Seal_Max(int a, int b);

void Seal_M4Identity(Seal_Matrix4x4 matrix) {
	_Seal_MatrixIdentity(matrix, 4);
}

void Seal_M4Ortho(Seal_Matrix4x4 matrix, Seal_Rect screen, Seal_Vector2 nearfar) {
	Seal_M4Identity(matrix);

	float rl = screen.size.x - screen.position.x;
	float tb = screen.size.y - screen.position.y;
	float fn = nearfar.x - nearfar.y;

	matrix[0]  = 2 / rl;
	matrix[5]  = 2 / tb;
	matrix[10] = 2 / fn;

	matrix[12] = -(screen.size.x + screen.position.x) / rl;   
	matrix[13] = -(screen.size.y + screen.position.y) / tb;   
	matrix[14] = -(nearfar.y + nearfar.x) / fn;

	matrix[15] = 1.f;
}

void Seal_M4Transpose(Seal_Matrix4x4 matrix) {
	_Seal_MatrixTranspose(matrix, matrix, 4);
}