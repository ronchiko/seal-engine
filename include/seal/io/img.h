#pragma once

typedef struct {
	int width;
	int height;
	int depth;

	int *buffer;
} Seal_Image;

Seal_Image Seal_LoadImage(const char *path);
void Seal_FreeImage(Seal_Image image);

