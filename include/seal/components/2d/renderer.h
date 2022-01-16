#pragma once

#include "seal/ecs/component.h"

SEAL_COMPONENT
typedef struct Seal_Renderer2d {
	Seal_Component base;

	Seal_Bool skip;
	Seal_ID program;
} Seal_Renderer2d;

SEAL_COMPONENT_BUFFER_INIT Seal_InitRenderer2dBuffer(void);
SEAL_COMPONENT_BUFFER_GET Seal_GetRenderer2dBuffer(void);
Seal_Renderer2d *Seal_AddRenderer2d(Seal_Entity entity, Seal_ID program);

void Seal_RemoveRenderer2d(Seal_Entity entity);

void Seal_ActivateRender2dSystem(void);