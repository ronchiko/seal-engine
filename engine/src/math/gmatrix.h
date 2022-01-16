#pragma once

void _Seal_MatrixBlit(float *matrix, float *in, int ow, int oh, int rw, int rh);
void _Seal_MatrixIdentity(float *matrix, int nelem);
void _Seal_MatrixTranspose(float *matrix, float *in, int n);
void _Seal_DrawMatrix(float *matrix, int w, int h);