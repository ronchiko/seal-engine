
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "seal/io.h" 

Seal_Size Seal_FConcatString(char **src, const char *cnt, Seal_Size srclen, Seal_Size cntlen) {
	*src = realloc(*src, srclen + cntlen + 1);
	memmove(*src + srclen, cnt, cntlen);
	*(*src +srclen + cntlen) = 0;
	return srclen + cntlen;
}

static Seal_Size SealIO_ReadBuffer(FILE *file, char *line, Seal_Size len) {
	int current = fgetc(file);
	Seal_Size index = 0;
	while(current >= 0 && index < len) {
		line[index++] = current;
		current = fgetc(file);
	}

	return (Seal_Size)index;
}

char *SealIO_ReadFile(const char *path) {
	// Check if the path is of an embbeded file or points to an archive file

	FILE *file; 
	errno_t err = fopen_s(&file, path, "r");
	if (err) {
		Seal_LogError("Failed to open file '%s'", SEAL_TRUE, path);
		return NULL;
	}

	Seal_Size bufferSize = 0, dynbufferSize = 0;
	char staticBuffer[512];
	char *dynamicBuffer = NULL;
	while((bufferSize = SealIO_ReadBuffer(file, staticBuffer, 512)) > 0) {
		dynbufferSize = Seal_FConcatString(&dynamicBuffer, staticBuffer, dynbufferSize, bufferSize);
	}

	return dynamicBuffer; 
}
