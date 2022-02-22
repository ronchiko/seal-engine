
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <GL/glew.h>

#include "const.h"

#include "seal/types/camera.h"
#include "seal/render2d/batcher.h"

#define BATCHER_GROWTH		 	5
#define QUADS_PER_BATCHER		500
#define QUAD_VERTECIES			4
#define BATCHER_VERTECIES		(QUADS_PER_BATCHER * QUAD_VERTECIES)

#if BATCHER_VERTECIES % 4 != 0 
#error The maximum number of vertecies of a batcher must be divisable by 4 
#endif

static struct {
	Seal_Batcher2d *batchers;
	Seal_Size used, alloced;
} gBatchers = {
	0, 0, 0
};

static Seal_Bool _IsBatcher(Seal_BatcherIndex i) {
	if(i < 0 || i >= gBatchers.used) return SEAL_FALSE;

	return gBatchers.batchers[i].vertecies != NULL;
}

static Seal_Bool _PrepareBatcherGl(Seal_Batcher2d *b2d, Seal_GL_Program program) {
	typedef Seal_Int GLLoc;
	assert(sizeof(Vertex) == 2 * sizeof(Seal_Vector2) + sizeof(Seal_Color) + 9 * sizeof(Seal_Float));
	
	typedef struct { const char *uniform; Seal_GL_UniformAddress *address; } Uniform;
	Uniform LOAD_UNIFORMS[] = {
		{ SEAL_SHADER_TEXTURE_PARAM,  		&b2d->uniforms.texture },
		{ SEAL_SHADER_CAMERA_MATRIX_PARAM,  &b2d->uniforms.camera },
	
		{ NULL, NULL }
	};

	for(Uniform *uni = LOAD_UNIFORMS; uni->address && uni->uniform ;++uni) {
		if((*(uni->address) = Seal_GL_ProgramUniformLocation(program, uni->uniform)) < 0) {
			Seal_LogError("Uniform '%s' is required for each shader program", uni->uniform);
			return SEAL_FALSE;
		}
	}

	GLLoc vertex 	= Seal_GL_ProgramAttribLocation(program, SEAL_SHADER_VERTEX_PARAM);
 	GLLoc uv 	 	= Seal_GL_ProgramAttribLocation(program, SEAL_SHADER_UV_PARAM);
	GLLoc transform = Seal_GL_ProgramAttribLocation(program, SEAL_SHADER_TRANSFORM_PARAM);
 	GLLoc tint 	 	= Seal_GL_ProgramAttribLocation(program, SEAL_SHADER_TINT_PARAM);

	GLLoc required[5] = { vertex, uv, transform, tint, 0xFF };
	for(int i = 0; i < 4; ++i)
		if(required[i] >= required[i + 1] || required[i] < 0) {
			Seal_LogError("Attributes '%s', '%s', '%s' and '%s' are must exists and be ordered for each shader program", 
				SEAL_SHADER_VERTEX_PARAM, SEAL_SHADER_UV_PARAM, SEAL_SHADER_TRANSFORM_PARAM, SEAL_SHADER_TINT_PARAM);
			return SEAL_FALSE;
		}

	GLCall(glGenVertexArrays(1, &b2d->gl.vao));
	GLCall(glBindVertexArray(b2d->gl.vao));

	GLCall(glGenBuffers(2, b2d->gl.buffers));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, b2d->gl.vbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, b2d->limit * sizeof(Vertex), NULL, GL_DYNAMIC_DRAW));
	
	assert(b2d->gl.vbo >= 0);
	Seal_GL_VAOContext ctx = Seal_GL_CreateVAOContext(b2d->gl.vao, sizeof(Vertex) / sizeof(Seal_Float));

	Seal_GL_VBOEnableVArray(&ctx, vertex, 2);
	Seal_GL_VBOEnableVArray(&ctx, uv, 2);
	Seal_GL_VBOEnbaleVArraysMatrixNxN(&ctx, transform, 3);
	Seal_GL_VBOEnableVArray(&ctx, tint, 4);

	uint32_t *indices = malloc(b2d->limit * 6 * sizeof(uint32_t));
	for(int i = 0, offsets[6] = { 0, 1, 2, 0, 3, 2 }; i < b2d->limit; ++i) {
		for(int j = 0; j < 6; ++j) 
			indices[6 * i + j] = i * 4 + offsets[j];
	}

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, b2d->gl.ibo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, b2d->limit * 6 * sizeof(uint32_t), indices, GL_STATIC_DRAW));

	free(indices);
	return SEAL_TRUE;
}

static Seal_Int _GetOrAllocateBatcher(void) {
	for(int i = 0; i < gBatchers.used; ++i) {
		if(gBatchers.batchers[i].vertecies == NULL) 
			return i;
	}

	if(gBatchers.used >= gBatchers.alloced) {
		gBatchers.batchers = realloc(gBatchers.batchers, sizeof(Seal_Batcher2d) * (gBatchers.alloced += BATCHER_GROWTH));
	}

	return gBatchers.used++;
} 


Seal_BatcherIndex Seal_CreateBatcher2d(Seal_Size vlimit, Seal_GL_Program program) {
	if (vlimit <= 0 || !glIsProgram(program)) return -1;

	Seal_BatcherIndex index = _GetOrAllocateBatcher();
	Seal_Batcher2d* b2d = &gBatchers.batchers[index];

	b2d->limit = vlimit; b2d->used = 0;
	if(!_PrepareBatcherGl(b2d, program)) {
		gBatchers.used--;
		return -1;
	}
	
	b2d->vertecies = malloc(vlimit * sizeof(Vertex));

	b2d->identifiers.program = program;
	b2d->identifiers.texture = SEAL_NO_TEXTURE;

	return index;
}


Seal_Batcher2d *Seal_GetBatcher2d(Seal_BatcherIndex index) {
	if(!_IsBatcher(index)) {
		return NULL;
	}

	return gBatchers.batchers + index;
}

void Seal_FreeBatcher2d(Seal_BatcherIndex i) {
	if(!_IsBatcher(i)) return;

	Seal_Batcher2d *b2d = gBatchers.batchers + i;

	free(b2d->vertecies); b2d->vertecies = NULL;
}

void Seal_FreeAllBatcher2ds(void) {
	for(Seal_BatcherIndex i = 0; i < gBatchers.used; ++i) {
		if(_IsBatcher(i)) Seal_FreeBatcher2d(i);
	}

	free(gBatchers.batchers);
	gBatchers.alloced = gBatchers.used = 0;
	gBatchers.batchers = NULL;
}

typedef const Seal_Batcher2d *restrict ReadonlyBatcher;

static Seal_Bool _CanFitInBatcher(
	ReadonlyBatcher b2d, Seal_GL_Program program, const Vertex *restrict vertex, Seal_GL_Texture texture
) {
	return 	b2d->used < b2d->limit && 
			b2d->identifiers.program == program && 
			(b2d->identifiers.texture == SEAL_NO_TEXTURE || b2d->identifiers.texture == texture);
}

static void _PushToBatcher(Seal_Batcher2d *restrict b2d, const Vertex *restrict vertex, Seal_GL_Texture texture) {
	b2d->identifiers.texture = texture;
	
	Vertex *glArray = b2d->vertecies;
	memcpy(glArray + b2d->used++, vertex, sizeof(Vertex));
}

Seal_Bool Seal_Batcher2dUploadVertex(Seal_GL_Program program, Seal_GL_Texture texture, const void *restrict _vert) {
	const Vertex *vertex = _vert;

	Seal_Batcher2d *b2d;
	for(Seal_BatcherIndex index = 0; index < gBatchers.used; ++index) {
		if(!_IsBatcher(index))
			continue;
		
		b2d = gBatchers.batchers + index;
		if(!_CanFitInBatcher(b2d, program, vertex, texture)) 
			continue;

		_PushToBatcher(b2d, _vert, texture);
		return SEAL_TRUE;
	}

	// Since this is a brand new batcher, we can push without checking if we can fit
	Seal_BatcherIndex i = Seal_CreateBatcher2d(BATCHER_VERTECIES, program);
	b2d = gBatchers.batchers + i;

	_PushToBatcher(b2d, _vert, texture);

	return SEAL_TRUE;
}

void Seal_Batcher2dPublish(void) {
	Seal_Matrix3x3 camera;
	Seal_M3Identity(camera);

	GLCall(glEnable(GL_TEXTURE_2D));
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	
	Seal_M3Camera(camera, Seal_GetMainCamera());
	Seal_M3Transpose(camera);

	for(Seal_BatcherIndex index = 0; index < gBatchers.used; ++index) {
		if (!_IsBatcher(index))
			continue;

		Seal_Batcher2d *b2d = gBatchers.batchers + index;
		Seal_GL_UseProgram(b2d->identifiers.program);

		// Push uniforms
		GLCall(glActiveTexture(GL_TEXTURE1));
		GLCall(glBindTexture(GL_TEXTURE_2D, b2d->identifiers.texture));
		GLCall(glUniform1i(b2d->uniforms.texture, 1));

		GLCall(glUniformMatrix3fv(b2d->uniforms.camera, 1, GL_FALSE, (float *)camera));

		// Link VAO
		GLCall(glBindVertexArray(b2d->gl.vao));

		GLCall(glBindBuffer(GL_ARRAY_BUFFER, b2d->gl.vbo));
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, b2d->used * sizeof(Vertex), b2d->vertecies));

		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, b2d->gl.ibo));
		GLCall(glDrawElements(GL_TRIANGLES, (b2d->used >> 2) * 6, GL_UNSIGNED_INT, NULL));

		// Reset buffer for next frame
		b2d->used = 0;
		b2d->identifiers.texture = SEAL_NO_TEXTURE;
	}

	// Reset OpenGL state after rendering
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	GLCall(glUseProgram(0));

	GLCall(glDisable(GL_TEXTURE_2D));
	GLCall(glDisable(GL_BLEND));
}