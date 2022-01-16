
#include <stdio.h>

#include "seal/math.h"

void _Seal_MatrixIdentity(float *matrix, int rows) {
	int g = 0;
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < rows; j++)
			matrix[i * rows + j] = (float)(i == j);
	}
}

void _Seal_DrawMatrix(float *matrix, int w, int h) {
	for(int i = 0; i < w; i++) {
		printf("[");
		for(int j = 0; j < h; j++)
			printf("%f ", matrix[j + i * w]);
		printf("]\n");
	}
	printf("\n");
}

void _Seal_MatrixBlit(float *matrix, float *in_matrix, int ow, int oh, int w, int h) {
	for(int i = 0; i < Seal_Min(ow, w); i++)
		for(int j = 0; j < Seal_Min(oh, h); j++)
			matrix[i + j * h] = in_matrix[i + j * oh];
}

void _Seal_MatrixTranspose(float *matrix, float *in, int n) {
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < i; j++) {
			float temp = in[j + i * n];
			matrix[j + i * n] = in[i + j * n];
			matrix[i + j * n] = temp;
		}
	}
}