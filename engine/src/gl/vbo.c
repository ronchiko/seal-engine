#pragma once

#include "seal/gl.h"

Seal_GL_VAOContext Seal_GL_CreateVAOContext(Seal_GL_VBO vao, Seal_Size vertexSize) {
	return (Seal_GL_VAOContext){
		.vao = vao,
		.vsize = vertexSize,
		.usedFloats = 0,
		.attrib = 0,
	};
}

void Seal_GL_VBOEnableVArray(Seal_GL_VAOContext *context, Seal_Int attribLoc, Seal_Int count) {
	GLCall(glEnableVertexArrayAttrib(context->vao, attribLoc));
	GLCall(glVertexAttribPointer(attribLoc, count, GL_FLOAT, GL_FALSE, context->vsize * sizeof(Seal_Float), 
		(const void *)(Seal_Long)(context->usedFloats * sizeof(Seal_Float))));

	context->usedFloats += count;
}

void Seal_GL_VBOEnbaleVArraysMatrixNxN(Seal_GL_VAOContext *context, Seal_Int attribLoc, Seal_Int n) {
	for(int i = 0; i < n; ++i) {
		Seal_GL_VBOEnableVArray(context, attribLoc + i, n);
	}
}