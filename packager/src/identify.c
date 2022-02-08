
#include <string.h>
#include <seal/io/fs.h>

#include "packager.h"


static const struct {char *key; Seal_FileGroup group;} EXTENSIONS[] = {
	// Shaders
	{"glsl", SEAL_GROUP_SHADER},
	{"vert", SEAL_GROUP_SHADER},
	{"frag", SEAL_GROUP_SHADER},

	// Images
	{"png", SEAL_GROUP_TEXTURE},
	{"jpg", SEAL_GROUP_TEXTURE},
	{"jpeg", SEAL_GROUP_TEXTURE},
	{"bmp", SEAL_GROUP_TEXTURE},

	// Text
	{"txt", SEAL_GROUP_GENERIC_TEXT},
	{"json", SEAL_GROUP_GENERIC_TEXT},
	{"yaml", SEAL_GROUP_GENERIC_TEXT},

	// Ignore
	{"pup", SEAL_GROUP_IGNORE},
	{"zip", SEAL_GROUP_IGNORE},
	{"exe", SEAL_GROUP_IGNORE},
	{"dll", SEAL_GROUP_IGNORE},
	{"lib", SEAL_GROUP_IGNORE},
	{"obj", SEAL_GROUP_IGNORE},
	{"o", SEAL_GROUP_IGNORE},
};

static const int EXTENSIONS_COUNT = sizeof(EXTENSIONS) / sizeof(EXTENSIONS[0]);

Seal_FileGroup Seal_GetFileGroup(const char *path, int maxl) {
	char ext[17];
	Seal_GetFileExt(path, maxl, ext, 16);
	ext[16] = 0;

	for(int i = 0; i < EXTENSIONS_COUNT; ++i) {
		if(strcmp(ext, EXTENSIONS[i].key) == 0)
			return EXTENSIONS[i].group;
	}

	return SEAL_GROUP_GENERIC_BIN;
}