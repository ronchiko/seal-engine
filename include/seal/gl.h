#pragma once

#include <GL/glew.h>

#include "common.h"

#ifdef _DEBUG
#define GLCall(fnc)	{ fnc; GLenum err = glGetError(); if(err != GL_NO_ERROR) \
	{ Seal_LogError("Error while executing '"#fnc"' %s\n", SEAL_TRUE, glewGetErrorString(err)); } }
#else
#define GLCall(fnc) fnc;
#endif


#define NO_PROGRAM 		0
#define NO_SHADER		0

typedef Seal_Int Seal_VAO;

typedef GLuint Seal_GL_Buffer;
typedef GLuint Seal_GL_Texture;

typedef Seal_GL_Buffer Seal_GL_VBO;

typedef struct {
	Seal_GL_VBO vbo;
	Seal_Size vsize;
	Seal_Size usedFloats;
	Seal_Size attrib;
} Seal_GL_VBOContext;

Seal_GL_VBOContext Seal_GL_CreateVBOContext(Seal_GL_VBO vbo, Seal_Size expectedVSize);

void Seal_GL_VBOEnableVArray(Seal_GL_VBOContext *context, Seal_Int attribLoc, Seal_Int count);
void Seal_GL_VBOEnbaleVArraysMatrixNxN(Seal_GL_VBOContext *context, Seal_Int attribLoc, Seal_Int n);

#include "seal/gl/shader.h"
#include "seal/gl/texture.h"