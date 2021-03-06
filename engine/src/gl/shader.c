
#include <stdlib.h>
#include <gl/glew.h>

#include "seal/gl.h"
#include "seal/io.h"

#define MAX_SHADER_LOADS 30

inline static GLuint Seal_GL_GLType2ShaderType(Seal_GL_ShaderType type) {
	switch(type) {
		case SEAL_SHADER_VERTEX: 	return GL_VERTEX_SHADER;
		case SEAL_SHADER_FRAGMENT: 	return GL_FRAGMENT_SHADER;
		case SEAL_SHADER_COMPUTE:	return GL_COMPUTE_SHADER;

		default: return 0;
	}
}  

Seal_GL_Shader Seal_GL_CompileShaderRaw(const char *source, Seal_GL_ShaderType type) {
	if (!source) {
		return (Seal_GL_Shader){ .id = SEAL_GL_NO_SHADER, .type = type };
	}
	
	GLuint _shader = glCreateShader(Seal_GL_GLType2ShaderType(type));

	glShaderSource(_shader, 1, (const GLchar **)&source, NULL);
	glCompileShader(_shader);

	GLuint successful;
	glGetShaderiv(_shader, GL_COMPILE_STATUS, &successful);
	if(successful != GL_TRUE) {
		Seal_LogError("Failed create shader from '%s'", source);

		GLint messageLength, realLength;
		glGetShaderiv(_shader, GL_INFO_LOG_LENGTH, &messageLength);

		char *msg = malloc(messageLength * sizeof(GLchar));
		glGetShaderInfoLog(_shader, messageLength, &realLength, msg);
		if(realLength > 0) Seal_Log(msg);

		glDeleteShader(_shader);
		return (Seal_GL_Shader){ .id = SEAL_GL_NO_SHADER, .type = type };
	}

	return (Seal_GL_Shader){ .id = _shader, .type = type };
}

Seal_GL_Shader Seal_GL_CompileShader(const char *source, Seal_GL_ShaderType type) {
	char *_source = SealIO_ReadFile(source).data;
	if (!_source) {
		Seal_LogError("Failed to read shader '%s'", source);
		return (Seal_GL_Shader){ .id = SEAL_GL_NO_SHADER, .type = type };
	}
	
	Seal_GL_Shader shader = Seal_GL_CompileShaderRaw(_source, type);
	free(_source);
	
	return shader;
}

Seal_GL_Program Seal_GL_LinkProgram(Seal_GL_Shader vrt, Seal_GL_Shader frg){
	GLuint program = glCreateProgram();
	if (vrt.type != SEAL_SHADER_VERTEX) {
		Seal_LogError("Shader %d is not a vertex shader", vrt.id);
		return 0;
	}
	if (frg.type != SEAL_SHADER_FRAGMENT) {
		Seal_LogError("Shader %d is not a fragment shader", frg.id);
		return 0;
	}

	glAttachShader(program, vrt.id);
	glAttachShader(program, frg.id);
	
	glLinkProgram(program);
	glValidateProgram(program);

	GLuint successful;
	glGetProgramiv(program, GL_LINK_STATUS, &successful);
	if(successful != GL_TRUE) {
		Seal_LogError("Failed to compile program from shader %d %d", vrt.id, frg.id);

		GLuint messageLength, realLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &messageLength);

		char *msg = malloc(messageLength * sizeof(GLchar));
		glGetProgramInfoLog(program, messageLength, &realLength, msg);
		if(realLength > 0) Seal_Log(msg);

		glDeleteProgram(program);
		return 0;
	} 

	return program;
}

void Seal_GL_DestroyShader(Seal_GL_Shader shdr) {
#if !defined(SEAL_UNSAFE_COMPILE)
	if(!glIsShader(shdr.id)) {
		Seal_LogWarning("ID %d is not a shader, wont be destroyed", shdr.id);
		return;
	}
#endif
	glDeleteShader(shdr.id);
}

void Seal_GL_DestroyProgram(Seal_GL_Program prg) {

#if !defined(SEAL_UNSAFE_COMPILE)
	if(!glIsProgram(prg)) {
		Seal_LogWarning("ID %d is not a shader, wont be destroyed", prg);
		return;
	}
#endif
	glDeleteProgram(prg);
}

Seal_Int Seal_GL_ProgramAttribLocation(Seal_GL_Program program, const char *name) {
	GLint offset;
	GLCall(offset = glGetAttribLocation(program, name));
	if (offset < 0) {
		Seal_LogError("Failed to find attribute '%s' in program %d", name, program);
		return -1;
	}

	return offset;
}

Seal_Int Seal_GL_ProgramUniformLocation(Seal_GL_Program program, const char *name) {
	GLint offset;
	offset = glGetUniformLocation(program, name);
	if (offset < 0) {
		Seal_LogError("Failed to find attribute '%s' in program %d", name, program);
		return -1;
	}

	return offset;
}

void Seal_GL_UseProgram(Seal_GL_Program program) {
	GLCall(glUseProgram(program));
}