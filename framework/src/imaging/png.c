
#include <png.h>
#include <string.h>

#include <seal/io/img.h>
#include <seal/io.h>
#include <seal/common.h>

#define _NORMALIZE_COLOR(c) ((c) / 255.f)

typedef struct {
	Seal_Size i;
	Seal_Size n;
	char *buffer;
} FileIncrementalReader;

static void _PngReadChunk(png_structp pngPtr, png_bytep out, png_size_t count) {
	png_voidp ioPtr = png_get_io_ptr(pngPtr);
	if(!ioPtr) return;

	FileIncrementalReader *ifr = (FileIncrementalReader *)ioPtr; 

	for(int i = 0; i < count && ifr->i < ifr->n; ++i, ++ifr->i) 
		out[i] = ifr->buffer[ifr->i];
}

void Seal_ReadRGBA(Seal_Image *img, png_structrp png, png_inforp info, Seal_Bool alpha) {
	const png_uint_32 bpr = png_get_rowbytes(png, info);
	const uint32_t totalBytes = img->depth * img->height * img->width * sizeof(Seal_Float) - 1;
	png_bytep bytes = malloc(bpr);
	
	for(Seal_Size row = img->height - 1; row >= 0; --row) {
		png_read_row(png, bytes, NULL);
		const png_uint_32 offset = row * img->width;

		png_uint_32 index = 0;
		for(Seal_Size col = 0; col < img->width; ++col) {
			const png_byte r = bytes[index++];
			const png_byte g = bytes[index++];
			const png_byte b = bytes[index++];
			const png_byte a = alpha ? bytes[index++] : 0xFF;

			img->buffer[col + offset] = r | (g << 8) | (b << 16) | (a << 24);
		}
	}

	free(bytes);
}

Seal_Bool Seal_LoadPngImage(Seal_Image *img, const char *path) {
	Seal_File file = SealIO_ReadFileRaw(path);
	if (!file.data || file.length <= 0) {
		return SEAL_FALSE;
	}


	enum { ePngSignatureLen = 8 };
	char signature[ePngSignatureLen];
	memcpy(signature, file.data, ePngSignatureLen * sizeof(char));

	if (!png_check_sig(signature, ePngSignatureLen)) {
		Seal_LogError("%s PNG has invalid signature", SEAL_FALSE, path);
		goto failure;
	}

	png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!pngPtr) {
		goto failure;
	}

	png_infop pngInfo = png_create_info_struct(pngPtr);
	if(!pngInfo) {
		png_destroy_read_struct(&pngPtr, NULL, NULL);
		goto failure;
	}
	
	if (setjmp(png_jmpbuf(pngPtr))) {
		Seal_LogError("Failed to read PNG %s", SEAL_FALSE, path);
		goto failure;
	}

	FileIncrementalReader reader = {
		.buffer = file.data + 8,
		.n = file.length,
		.i = 0
	};

	png_set_read_fn(pngPtr, &reader, &_PngReadChunk);
	png_set_sig_bytes(pngPtr, 8);

	png_read_info(pngPtr, pngInfo);

	png_uint_32 width = 0, height = 0;
	int bitDepth = 0, colorType = -1;	
	if(png_get_IHDR(pngPtr, pngInfo, &width, &height, &bitDepth, &colorType, NULL, NULL, NULL) != 1) {
		png_destroy_read_struct(&pngPtr, &pngInfo, NULL);
		goto failure;
	}

	Seal_Log("Assigning values [%d, %d]: %d", width, height, colorType);
	img->height = height;
	img->width = width;
	img->depth = 4;
	img->buffer = malloc(sizeof(Seal_Float) * width * height);

	switch(colorType) {	
	case PNG_COLOR_TYPE_PALETTE:
	case PNG_COLOR_TYPE_RGBA:
		Seal_ReadRGBA(img, pngPtr, pngInfo, SEAL_TRUE);
		break;
	case PNG_COLOR_TYPE_RGB:
		Seal_ReadRGBA(img, pngPtr, pngInfo, SEAL_FALSE);
		break;
	default:
		Seal_LogError("Invalid color type %d for PNG %s", SEAL_FALSE, colorType, path);
		break;
	}

	png_destroy_read_struct(&pngPtr, &pngInfo, NULL);
	return SEAL_TRUE;

failure:
	Seal_LogError("Failed to load PNG '%s'", SEAL_FALSE, path);
	free(file.data);
	return SEAL_FALSE;
}