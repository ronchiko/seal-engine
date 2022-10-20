
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <seal/common.h>

void *Seal_DeepCopy(void *src, size_t size) {
	void *dst = malloc(size);
	return memcpy(dst, src, size);
}