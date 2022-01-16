
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

	Seal_TransformComponent *transform = (Seal_TransformComponent *)components[transformOffset];
	Seal_Renderer2d *renderer = (Seal_Renderer2d *)components[rendererOffset];

	if (!transform || !renderer) return;

	Seal_Matrix3x3 matrix;
	Seal_M3Identity(matrix);
	Seal_M3Transform(matrix, transform->position, transform->scale, transform->angle);
	Seal_M3Transpose(matrix);

	Seal_Batcher2d *batcher = Seal_GetBatcher2d(gBatcher);

	Seal_Batcher2dPushVector2(batcher, (Seal_Vector2){-0.5f, -0.5f});
	Seal_Batcher2dPushM3(batcher, matrix);
	
	Seal_Batcher2dPushVector2(batcher, (Seal_Vector2){0.5f, -0.5f});
	Seal_Batcher2dPushM3(batcher, matrix);
	
	Seal_Batcher2dPushVector2(batcher, (Seal_Vector2){0.5f, 0.5f});
	Seal_Batcher2dPushM3(batcher, matrix);
	
	Seal_Batcher2dPushVector2(batcher, (Seal_Vector2){-0.5f, 0.5f});
	Seal_Batcher2dPushM3(batcher, matrix);
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
	} Vertex;

	if(gGLDefaults.program == SEAL_GL_NO_PROGRAM) {
		gGLDefaults.vertex = Seal_GL_CompileShader  ("assets/vertex.glsl", SEAL_SHADER_VERTEX);
		gGLDefaults.fragment = Seal_GL_CompileShader("assets/fragment.glsl", SEAL_SHADER_FRAGMENT);

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

	gBatcher = Seal_CreateBatcher2d(sizeof(Vertex), 200);

	Seal_Int vertexLoc = Seal_GL_ProgramAttribLocation(gGLDefaults.program, "vertex");
	Seal_Int matrixLoc = Seal_GL_ProgramAttribLocation(gGLDefaults.program, "transform");
	if(vertexLoc < 0 || matrixLoc < 0) {
		return;
	}

	Seal_GL_VBOContext glVbo;
	if((glVbo = Seal_Batcher2dGetVBOContext(gBatcher)).vbo >= 0) {
		
		Seal_GL_VBOEnableVArray(&glVbo, vertexLoc, 2);
		Seal_GL_VBOEnbaleVArraysMatrix(&glVbo, matrixLoc, 3);
	}

	Seal_ActivateSystem(&system);
}