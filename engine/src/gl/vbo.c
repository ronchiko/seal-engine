#pragma once

#include "seal/gl.h"

Seal_GL_VBOContext Seal_GL_CreateVBOContext(Seal_GL_VBO vbo, Seal_Size vertexSize) {
	return (Seal_GL_VBOContext){
		.vbo = vbo,
		.vsize = vertexSize,
		.usedFloats = 0,
		.attrib = 0,
	};
}

void Seal_GL_VBOEnableVArray(Seal_GL_VBOContext *context, Seal_Int attribLoc, Seal_Int count) {
	GLCall(glEnableVertexArrayAttrib(context->vbo, attribLoc));
	GLCall(glVertexAttribPointer(attribLoc, count, GL_FLOAT, GL_FALSE, context->vsize * sizeof(Seal_Float), 
		(const void *)(Seal_Long)(context->usedFloats * sizeof(Seal_Float))));

	context->usedFloats += count;
	Seal_Log("Registering %d, used %d, offset %d [%d]", attribLoc, 
		context->usedFloats, 
		context->usedFloats * sizeof(Seal_Float), 
		context->vsize * sizeof(Seal_Float));
}

void Seal_GL_VBOEnbaleVArraysMatrixNxN(Seal_GL_VBOContext *context, Seal_Int attribLoc, Seal_Int n) {
	for(int i = 0; i < n; ++i) {
		Seal_GL_VBOEnableVArray(context, attribLoc + i, n);
	}
}