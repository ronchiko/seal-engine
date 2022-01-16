
#include <string.h>

#include "seal/math.h"
#include "gmatrix.h"

void Seal_M3Identity(Seal_Matrix3x3 matrix){
	_Seal_MatrixIdentity(matrix, 3);
}

void Seal_M3Rotation(Seal_Matrix3x3 matrix, float angle) {
	Seal_M3Identity(matrix);

	Seal_Matrix2x2 _2d_rotation;
	Seal_M2Rotation(_2d_rotation, angle);
	_Seal_MatrixBlit(matrix, _2d_rotation, 2, 2, 3, 3);
}

void Seal_M3Translate(Seal_Matrix3x3 matrix, float x, float y) {
	Seal_M3Identity(matrix);

	matrix[2] = x;
	matrix[5] = y;
}

void Seal_M3Scale(Seal_Matrix3x3 matrix, float x, float y) {
	Seal_M3Identity(matrix);

	matrix[0] = x;
	matrix[4] = y;
}

void Seal_M3Multiply(Seal_Matrix3x3 out, Seal_Matrix3x3 a, Seal_Matrix3x3 b) {
	float buffer[9];
	
	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			buffer[i + 3 * j] = a[i] * b[3 * j] + a[i + 3 * 1] * b[1 + 3 * j] + a[i + 3 * 2] * b[3 * j + 2];

	memcpy(out, buffer, 9 * sizeof(float));
}

void Seal_M3Transform(Seal_Matrix3x3 matrix, Seal_Vector2 p, Seal_Vector2 s, float a) {
	Seal_M3Identity(matrix);

	Seal_Matrix3x3 scale;		Seal_M3Scale(scale, s.x, s.y);
	Seal_Matrix3x3 rotation;	Seal_M3Rotation(rotation, a);
	Seal_Matrix3x3 transform;	Seal_M3Translate(transform, p.x, p.y);

	Seal_M3Multiply(matrix, matrix, scale);
	Seal_M3Multiply(matrix, matrix, rotation);
	Seal_M3Multiply(matrix, matrix, transform);
}

void Seal_M3Transpose(Seal_Matrix3x3 matrix) {
	_Seal_MatrixTranspose(matrix, matrix, 3);
}