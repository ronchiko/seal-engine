#pragma once

#include "seal/common.h"

#include "seal/gl.h"
#include "seal/math.h"

typedef struct Seal_Batcher2d {
	Seal_Size limitFP;		// The upper limit of floats stored in this batcher	
	Seal_Size usedFP;		// The current amount of floats stored in this batcher
	Seal_Size vertexSizeFP;	// The size of each vertex (in floats)
	Seal_Float *vertecies;	// The buffer

	struct  {
		union {
			Seal_GL_Buffer buffers[2];
			struct { Seal_GL_Buffer vbo; Seal_GL_Buffer ibo; };
		};
		Seal_GL_Buffer vao;
	} gl;
} Seal_Batcher2d;

typedef uint16_t Seal_BatcherIndex;

/** Global Batcher 2d operation **/

/* Create a new 2d batcher */ 
Seal_BatcherIndex Seal_CreateBatcher2d(Seal_Size vertexSizeFP, Seal_Size drawLimit);
/* Free batcher 2d using its index */ 
void Seal_FreeBatcher2d(Seal_BatcherIndex index);
/* Frees all batcher 2d object */ 
void Seal_FreeAllBatcher2ds(void);
/* Checks if a batcher index is a valid index */
Seal_Bool Seal_IsBatcher2d(Seal_BatcherIndex index);
/* Gets the most empty batcher out of the allocated batchers */
Seal_Batcher2d *Seal_GetBatcher2d(Seal_BatcherIndex index);

/** Batcher 2d methods **/
void Seal_Batcher2dPushVector2(Seal_Batcher2d *batcher, Seal_Vector2 vector2);
void Seal_Batcher2dPushColor(Seal_Batcher2d *batcher, Seal_Color color);
void Seal_Batcher2dPushInt(Seal_Batcher2d *batcher, Seal_Int texture);
void Seal_Batcher2dPushM3(Seal_Batcher2d *batcher, Seal_Matrix3x3 m3);

Seal_GL_VBOContext Seal_Batcher2dGetVBOContext(Seal_BatcherIndex batcher);

void Seal_Batcher2dUpload(Seal_Batcher2d *);
void Seal_Batcher2dReset(Seal_Batcher2d *);