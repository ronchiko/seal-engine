#pragma once

#include <string.h>
#include <ctype.h>

#include <seal/io/img.h>
#include <seal/io/fs.h>
#include <seal/io.h>

#include <png.h>

enum Seal_ImageFormat {
	SEAL_INVALID_IMAGE_FMT,
	
	SEAL_PNG,
	SEAL_JPG,
	SEAL_BMP
};

typedef uint64_t Seal_HashResult;

static enum Seal_ImageFormat _DetectFormat(const char *ext) {
	char normalized[64];
	
	int i;
	for(i = 0; i < 63 && *ext; ++i, ++ext) normalized[i] = tolower(*ext);
	normalized[i] = 0;

	static struct {char *ext; enum Seal_ImageFormat fmt; } gExtensions[] = {
		{ "bmp", SEAL_BMP },
		{ "png", SEAL_PNG },
		{ "jpeg", SEAL_JPG },
		{ "jpg", SEAL_JPG },
	};
	static const int gExtenstionsCount = sizeof(gExtensions) / sizeof(gExtensions[0]);

	for (int i = 0; i < gExtenstionsCount; ++i) {
		if(!strcmp(gExtensions[i].ext, normalized)) 
			return gExtensions[i].fmt;
	}

	return SEAL_INVALID_IMAGE_FMT;
}

void Seal_FreeImage(Seal_Image image) {
	free(image.buffer);
}


Seal_Image Seal_LoadImage(const char *path) {
	extern Seal_Bool Seal_LoadPngImage(Seal_Image *img, const char *path);

	char ext[64];
	int length = Seal_GetFileExt(path, 256, ext, 63);

	enum Seal_ImageFormat format = _DetectFormat(ext);
	Seal_Image img = { 0, 0, 0, NULL };

	switch(format) {
	case SEAL_PNG:
		if(!Seal_LoadPngImage(&img, path)) {
			Seal_LogError("Failed to open PNG file");
			break;
		}
		break;
	default:
		Seal_LogError("Invalid file format!");
		break;
	}

	return img;
}