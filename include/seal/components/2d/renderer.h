#pragma once

#include "seal/ecs/component.h"

typedef struct Seal_Renderer2d {
	Seal_Component base;

	Seal_Bool skip;

	Seal_Color tint;
	Seal_Texture textureId;
} Seal_Renderer2d;


Seal_Renderer2d *Seal_AddRenderer2d(Seal_Entity entity, Seal_ID program);

void Seal_RemoveRenderer2d(Seal_Entity entity);


#include <seal/systems/render2d.h>