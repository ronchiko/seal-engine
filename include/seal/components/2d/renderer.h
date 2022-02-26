#pragma once

#include <seal/math.h>
#include <seal/ecs/component.h>

#include <seal/gl.h>

typedef struct Seal_Renderer2d {
	Seal_Component base;

	Seal_Bool skip;
	Seal_GL_Program program;

	Seal_Color tint;
	Seal_Vector4 uv;
	Seal_TextureId textureId;
} Seal_Renderer2d;


Seal_Renderer2d *Seal_AddRenderer2d(Seal_Entity entity, Seal_ID program);
void Seal_RemoveRenderer2d(Seal_Entity entity);

void Seal_InitRenderer2dBuffer(void);
Seal_ComponentBuffer Seal_GetRenderer2dBuffer(void);

void Seal_ActivateRender2dSystem(void);