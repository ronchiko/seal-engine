
#include "seal/gl.h"

#include "seal/components/2d/renderer.h"

static Seal_ComponentBuffer gRender2dBuffer;

Seal_ComponentBuffer Seal_GetRenderer2dBuffer(void) {
	return gRender2dBuffer;
}

void Seal_InitRenderer2dBuffer(void) {
	gRender2dBuffer = Seal_CreateComponentBuffer(sizeof(Seal_Renderer2d));
}

Seal_Renderer2d *Seal_AddRenderer2d(Seal_Entity entity, Seal_GL_Program program) {

	Seal_Renderer2d r2d = {
		.base = { 0, 0 },
		.skip = SEAL_FALSE,
		.tint = { 1.f, 1.f, 1.f, 1.f },
		.uv = { 0, 0, 1, 1 },
		.textureId = 0
	};

	Seal_AttachComponent(entity, &r2d.base);
	return (Seal_Renderer2d *)Seal_AddComponent(gRender2dBuffer, &r2d.base);
}

void Seal_RemoveRenderer2d(Seal_Entity entity) {
	Seal_RemoveComponent(gRender2dBuffer, entity);
}