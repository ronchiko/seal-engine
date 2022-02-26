#pragma once

#include "seal/common.h"

#include "seal/gl.h"
#include "seal/math.h"

typedef struct Seal_Batcher2d {
	Seal_Size limit;		// The upper limit of floats stored in this batcher	
	Seal_Size used;		// The current amount of floats stored in this batcher
	void *vertecies;	// The buffer

	struct  {
		union {
			Seal_GL_Buffer buffers[2];
			struct { 
				Seal_GL_VBO vbo; 
				Seal_GL_Buffer ibo;
			};
		};
		Seal_GL_Buffer vao;
	} gl;

	struct {
		Seal_GL_Program program;
		Seal_GL_Texture texture;
	} identifiers;

	struct {
		Seal_GL_UniformAddress texture;
		Seal_GL_UniformAddress camera;
	} uniforms;
} Seal_Batcher2d;

typedef int16_t Seal_BatcherIndex;

/** Global Batcher 2d operation **/

Seal_BatcherIndex Seal_CreateBatcher2d(Seal_Size vertexLimit, Seal_GL_Program program);

Seal_Batcher2d *Seal_GetBatcher2d(Seal_BatcherIndex);

void Seal_FreeAllBatcher2ds(void);
void Seal_FreeBatcher2d(Seal_BatcherIndex);

Seal_GL_VAOContext Seal_Batcher2dGetVBOContext(Seal_BatcherIndex batcher);

Seal_Bool Seal_Batcher2dUploadVertex(Seal_GL_Program program, Seal_GL_Texture texture, const void *restrict vertex);

void Seal_Batcher2dPublish(void);