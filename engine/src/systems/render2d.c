
#include <GL/glew.h>

#include "seal/gl.h"
#include "seal/ecs/system.h"

#include "seal/components/2d/renderer.h"
#include "seal/components/2d/transform.h"

#include "seal/render2d/batcher.h"

static Seal_BatcherIndex gBatcher; 

struct {
	Seal_GL_Shader vertex;
	Seal_GL_Shader fragment;
	Seal_GL_Program program;
} gGLDefaults = { 0, 0, 0 };

Seal_GL_Program Seal_GL_DefaultProgram(void) {
	return gGLDefaults.program;
} 

void Seal_Renderer2dSystem(Seal_Component **components, const Seal_Size count) {	
	static const Seal_Size transformOffset = 0; 
	static const Seal_Size rendererOffset = 1; 
	static const Seal_Vector2 quad[4] = {
		{-0.5f, -0.5f}, {0.5f, -0.5f}, {0.5f, 0.5f}, {-0.5f, 0.5f}
	};

	Seal_Transform2d *transform = (Seal_Transform2d *)components[transformOffset];
	Seal_Renderer2d *renderer = (Seal_Renderer2d *)components[rendererOffset];

	if (!transform || !renderer) return;

	Seal_Matrix3x3 matrix;
	Seal_M3Identity(matrix);
	Seal_M3Transform(matrix, transform->position, transform->scale, transform->angle);
	Seal_M3Transpose(matrix);

	Seal_Batcher2d *batcher = Seal_GetBatcher2d(gBatcher);

	for(int i = 0; i < 4; ++i) {
		Seal_Batcher2dPushVector2(batcher, quad[i]);
		Seal_Batcher2dPushM3(batcher, matrix);
		Seal_Batcher2dPushColor(batcher, renderer->tint);
	}	
}

void Seal_Renderer2dFinalize(void) {
	Seal_Batcher2d *batcher = Seal_GetBatcher2d(gBatcher);
	Seal_GL_UseProgram(gGLDefaults.program);

	Seal_Batcher2dUpload(batcher);
}

void Seal_Renderer2dCleanup(void) {
	Seal_FreeBatcher2d(gBatcher);

	Seal_GL_DestroyShader(gGLDefaults.vertex);
	Seal_GL_DestroyShader(gGLDefaults.fragment);
	Seal_GL_DestroyProgram(gGLDefaults.program);
}

void Seal_ActivateRender2dSystem(void) {
	typedef struct {
		Seal_Vector2 vertex;
		Seal_Matrix3x3 transform;
		Seal_Color color;
	} Vertex;

	if(gGLDefaults.program == SEAL_GL_NO_PROGRAM) {
		gGLDefaults.vertex = Seal_GL_CompileShader  ("assets/shaders/vertex.glsl", SEAL_SHADER_VERTEX);
		gGLDefaults.fragment = Seal_GL_CompileShader("assets/shaders/fragment.glsl", SEAL_SHADER_FRAGMENT);

		gGLDefaults.program = Seal_GL_LinkProgram(gGLDefaults.vertex, gGLDefaults.fragment);
	}

	Seal_ComponentBuffer buffers[2] = {
		Seal_GetTransformBuffer(),
		Seal_GetRenderer2dBuffer()
	};
	
	Seal_System system = {
		.type = SEAL_UPDATE_SYSTEM,
		.required = {
			.components = buffers,
			.count = 2
		},
		.handler = &Seal_Renderer2dSystem,
		.stateless_handlers.cleanup = &Seal_Renderer2dCleanup,
		.stateless_handlers.finalize = &Seal_Renderer2dFinalize
	};

	gBatcher = Seal_CreateBatcher2d(sizeof(Vertex), 4000);

	Seal_Int vertexLoc 	= Seal_GL_ProgramAttribLocation(gGLDefaults.program, "vertex");
	Seal_Int matrixLoc 	= Seal_GL_ProgramAttribLocation(gGLDefaults.program, "transform");
	Seal_Int colorLoc 	= Seal_GL_ProgramAttribLocation(gGLDefaults.program, "color");
	if(vertexLoc < 0 || matrixLoc < 0 || colorLoc < 0) {
		Seal_LogError("Failed to loaded shader, missing attributes", SEAL_FALSE);
		return;
	}

	Seal_GL_VBOContext glVbo;
	if((glVbo = Seal_Batcher2dGetVBOContext(gBatcher)).vbo >= 0) {
		
		Seal_GL_VBOEnableVArray(&glVbo, vertexLoc, 2);
		Seal_GL_VBOEnbaleVArraysMatrixNxN(&glVbo, matrixLoc, 3);
		Seal_GL_VBOEnableVArray(&glVbo, colorLoc, 4);
	}

	Seal_ActivateSystem(&system);
}