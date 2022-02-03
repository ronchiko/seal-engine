
#include <stdlib.h>
#include <string.h>

#include <seal/fs.h>

int Seal_GetFileName(const char *path, int maxPathLen, char *buf, int bufSize) {
	const char *lastSperator = path;
	
	for(int i = 0; i < maxPathLen && *path; ++path, ++i) {
		if(*path == '/' || *path == '\\') lastSperator = path;
	}

	int length = path - lastSperator - 1;
	memcpy(buf, lastSperator + 1, sizeof(char) * min(length, bufSize));
	buf[length] = 0;

	return length;
}

int Seal_GetFileExt(const char *path, int maxPathLen, char *buf, int bufSize) {
	const char *dot = path;
	
	for(int i = 0; i < maxPathLen && *path; ++path, ++i) {
		if(*path == '.') {
			dot = path;
		}
	}

	int length = path - dot - 1;
	memcpy(buf, dot + 1, sizeof(char) * min(length, bufSize));
	buf[length] = 0;
	
	return length;
}

#if defined(_WIN32) || defined(_WIN64)
#include "windows.c.i"
#else
#error File iterator is not implemented for this system architecture
#endif