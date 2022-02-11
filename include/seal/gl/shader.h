#pragma once

#include "seal/common.h"

#define SEAL_GL_NO_SHADER 		0
#define SEAL_GL_NO_PROGRAM 		0

typedef enum Seal_GL_ShaderType {
	SEAL_SHADER_VERTEX,
	SEAL_SHADER_COMPUTE,
	SEAL_SHADER_FRAGMENT
} Seal_GL_ShaderType;

typedef struct {
	Seal_ID id;
	Seal_GL_ShaderType type;
} Seal_GL_Shader;

typedef Seal_ID Seal_GL_Program;

Seal_GL_Shader Seal_GL_CompileShader(const char *source, Seal_GL_ShaderType type);
Seal_GL_Program Seal_GL_LinkProgram(Seal_GL_Shader vrt, Seal_GL_Shader frg);

void Seal_GL_DestroyProgram(Seal_GL_Program id);
void Seal_GL_DestroyShader(Seal_GL_Shader shader);

Seal_Int Seal_GL_ProgramAttribLocation(Seal_GL_Program progam, const char *name);
Seal_Int Seal_GL_ProgramUniformLocation(Seal_GL_Program progam, const char *name);

Seal_GL_Program Seal_GL_DefaultProgram(void);
void Seal_GL_UseProgram(Seal_GL_Program);
