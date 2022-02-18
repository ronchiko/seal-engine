#pragma once

#include <stdint.h>

#define SEAL_NO_TEXTURE 		0

typedef uint32_t Seal_TextureId;

Seal_TextureId Seal_CreateTextureFromAsset(const char *path, Seal_Int2 *size);

void Seal_FreeTexture(Seal_TextureId);