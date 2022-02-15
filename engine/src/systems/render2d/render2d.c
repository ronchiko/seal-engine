
#include <string.h>
#include <GL/glew.h>

#include <seal/gl.h>
#include <seal/ecs/system.h>

#include <seal/components/2d/renderer.h>
#include <seal/components/2d/transform.h>

#include "seal/render2d/batcher.h"
#include "const.h"

static struct {
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
	static const int uvs[8] = {
		0, 3,
		2, 3,
		2, 1,
		0, 1	
	};

	Seal_Transform2d *transform = (Seal_Transform2d *)components[transformOffset];
	Seal_Renderer2d *renderer = (Seal_Renderer2d *)components[rendererOffset];

	if (!transform || !renderer) return;

	Seal_Matrix3x3 matrix;
	Seal_M3Identity(matrix);
	Seal_M3Transform(matrix, transform->position, transform->scale, transform->angle);
	Seal_M3Transpose(matrix);

	Seal_GL_Program program = renderer->program ? renderer->program : gGLDefaults.program;

	Vertex vertex = { 0 };
	memcpy(vertex.transform, matrix, sizeof(Seal_Matrix3x3));
	vertex.tint = renderer->tint;

	for(int i = 0; i < 4; ++i) {
		vertex.vertex = quad[i];
		vertex.uv = (Seal_Vector2){ renderer->uv.array[uvs[i << 1]], renderer->uv.array[uvs[(i << 1) + 1]]};

		Seal_Batcher2dUploadVertex(program, renderer->textureId, &vertex);
	}
}

void Seal_Renderer2dFinalize(void) {
	Seal_Batcher2dPublish();
}

void Seal_Renderer2dCleanup(void) {
	Seal_FreeAllBatcher2ds();

	Seal_GL_DestroyShader(gGLDefaults.vertex);
	Seal_GL_DestroyShader(gGLDefaults.fragment);
	Seal_GL_DestroyProgram(gGLDefaults.program);
}

void Seal_ActivateRender2dSystem(void) {
	if(gGLDefaults.program == SEAL_GL_NO_PROGRAM) {
		extern Seal_GL_Shader Seal_GL_CompileShaderRaw(const char *source, Seal_GL_ShaderType type);
		Seal_Log("Loading in fallback shaders");

		gGLDefaults.vertex = Seal_GL_CompileShaderRaw(SEAL_VERTEX_SHADER_FALLBACK, SEAL_SHADER_VERTEX);
		gGLDefaults.fragment = Seal_GL_CompileShaderRaw(SEAL_FRAGMENT_SHADER_FALLBACK, SEAL_SHADER_FRAGMENT);

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
		.stateless_handlers.finalize = &Seal_Batcher2dPublish
	};

	Seal_ActivateSystem(&system);
}