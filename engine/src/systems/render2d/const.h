#pragma once

#include <seal/math.h>
#include <seal/gl.h>

typedef struct {
	Seal_Vector2 vertex;
	Seal_Vector2 uv;
	Seal_Matrix3x3 transform;
	Seal_Color tint;
} Vertex;

// Must be in every shader in this order!
extern const char *SEAL_SHADER_VERTEX_PARAM;
extern const char *SEAL_SHADER_UV_PARAM;
extern const char *SEAL_SHADER_TRANSFORM_PARAM;
extern const char *SEAL_SHADER_TINT_PARAM;
// Uniform
extern const char *SEAL_SHADER_TEXTURE_PARAM;
extern const char *SEAL_SHADER_CAMERA_MATRIX_PARAM;


extern const char *SEAL_VERTEX_SHADER_FALLBACK, *SEAL_FRAGMENT_SHADER_FALLBACK;