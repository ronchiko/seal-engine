
#include <stdlib.h>
#include <string.h>

#include "seal/render2d/batcher.h"


static Seal_Batcher2d *gBatchers = NULL;
static Seal_Size gAllocated = 0;


Seal_Bool Seal_IsBatcher2d(Seal_BatcherIndex index) {
	return gAllocated > index && gBatchers[index].limitFP > 0; 
}

Seal_Batcher2d *Seal_GetBatcher2d(Seal_BatcherIndex index) {
	return Seal_IsBatcher2d(index) ? &gBatchers[index] : NULL;
}

void Seal_FreeBatcher2d(Seal_BatcherIndex index) {
	if(Seal_IsBatcher2d(index)) {
		gBatchers[index].limitFP = gBatchers[index].usedFP = 0;

		glDeleteVertexArrays(1, &gBatchers[index].gl.vao);
		glDeleteBuffers(2, gBatchers[index].gl.buffers);

		free(gBatchers[index].vertecies);
		gBatchers[index].vertecies = NULL;
	}
}

void Seal_FreeAllBatcher2ds(void) {
	for(Seal_Size i = 0; i < gAllocated; ++i)
		Seal_FreeBatcher2d(i);

	free(gBatchers); 
	gAllocated = 0; 
	gBatchers = NULL;
}

static Seal_Size _FindOrAllocateBatcher() {
	for(Seal_Size i = 0; i < gAllocated; ++i)
		if(gBatchers[i].limitFP <= 0) return i;

	gBatchers = realloc(gBatchers, sizeof(Seal_Batcher2d) * (++gAllocated));
	return gAllocated - 1;
}

Seal_BatcherIndex Seal_CreateBatcher2d(Seal_Size vertexSize, Seal_Size limit) {
	Seal_Size index = _FindOrAllocateBatcher();

	const Seal_Size vertexSizeFP = vertexSize / sizeof(Seal_Float);
	const Seal_Size limitFP = limit * vertexSizeFP; 

	gBatchers[index].limitFP = limitFP;
	gBatchers[index].usedFP = 0;
	gBatchers[index].vertecies = malloc(vertexSize * limit);
	gBatchers[index].vertexSizeFP = vertexSizeFP;
	Seal_Log("LIMIT: %d, %d", limitFP, limit);
	const Seal_Size gIndeciesLimit = limit * sizeof(GLuint) * 6;
	GLuint *indecies = malloc(gIndeciesLimit);
	for(Seal_Size i = 0; i < limit; ++i) {
		indecies[6 * i + 0] = i * 4 + 0;
		indecies[6 * i + 1] = i * 4 + 1;
		indecies[6 * i + 2] = i * 4 + 2;

		indecies[6 * i + 3] = i * 4 + 0;
		indecies[6 * i + 4] = i * 4 + 3;
		indecies[6 * i + 5] = i * 4 + 2;
	}

	GLCall(glCreateVertexArrays(1, &gBatchers[index].gl.vao));
	GLCall(glBindVertexArray(gBatchers[index].gl.vao));

	GLCall(glGenBuffers(2, gBatchers[index].gl.buffers));

	// IBO
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gBatchers[index].gl.ibo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, gIndeciesLimit, indecies, GL_STATIC_DRAW));

	// VBO
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, gBatchers[index].gl.vbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, limitFP * sizeof(Seal_Float), NULL, GL_DYNAMIC_DRAW));

	
	free(indecies);
	return index;
}


void Seal_Batcher2dPushColor(Seal_Batcher2d *batcher, Seal_Color color) {
	if (!batcher) {
		DBG_WARN("Pushing vector2 to NULL batcher");
		return;
	}

	if(batcher->limitFP < batcher->usedFP + 4) {
		DBG_ERROR("Batcher limit has exceeded");
		return;
	}

	for(int i = 0; i < 4; ++i)
		batcher->vertecies[batcher->usedFP + i] = color.rgba[i];

	batcher->usedFP += 4;
}


void Seal_Batcher2dPushVector2(Seal_Batcher2d *batcher, Seal_Vector2 v2) {
	if (!batcher) {
		DBG_WARN("Pushing vector2 to NULL batcher");
		return;
	}

	if(batcher->limitFP < batcher->usedFP + 2) {
		DBG_ERROR("Batcher limit has exceeded");
		return;
	}

	batcher->vertecies[batcher->usedFP++] = v2.x;
	batcher->vertecies[batcher->usedFP++] = v2.y;
}

void Seal_Batcher2dPushInt(Seal_Batcher2d *batcher, Seal_Int integer) {
	if (!batcher) {
		DBG_WARN("Pushing integer to NULL batcher");
		return;
	}

	if(batcher->limitFP < batcher->usedFP + 1) {
		DBG_ERROR("Batcher limit has exceeded");
		return;
	}

	((Seal_Int *)batcher->vertecies)[batcher->usedFP++] = integer;
}

void Seal_Batcher2dPushM3(Seal_Batcher2d *batcher, Seal_Matrix3x3 m3) {
	if (!batcher) {
		DBG_WARN("Pushing integer to NULL batcher");
		return;
	}

	if(batcher->limitFP < batcher->usedFP + 9) {
		DBG_ERROR("Batcher limit has exceeded");
		return;
	}

	memcpy(batcher->vertecies + batcher->usedFP, m3, sizeof(Seal_Matrix3x3));
	batcher->usedFP += 9;
}

void Seal_Batcher2dUpload(Seal_Batcher2d *b2d) {
	const Seal_Size lUsedVertecies = b2d->usedFP / b2d->vertexSizeFP;
	
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, b2d->gl.vbo));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, b2d->usedFP * sizeof(Seal_Float), b2d->vertecies));

	GLCall(glBindVertexArray(b2d->gl.vao));
	GLCall(glDrawElements(GL_TRIANGLES, 6 * (lUsedVertecies >> 2), GL_UNSIGNED_INT, NULL));

	// Reset buffer
	b2d->usedFP = 0;
}

Seal_GL_VBOContext Seal_Batcher2dGetVBOContext(Seal_BatcherIndex b) {
	if(!Seal_IsBatcher2d(b)) {
		DBG_ERROR("%u is not a valid batcher id", b);
		return Seal_GL_CreateVBOContext(-1, 0);
	}

	return Seal_GL_CreateVBOContext(gBatchers[b].gl.vbo, gBatchers[b].vertexSizeFP);
}
