
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <seal/common.h>

void Seal_Log(const char *fmt, ...) {
	va_list _args;
	va_start(_args, fmt);
	
	vprintf(fmt, _args);
	printf("\n");
	fflush(stdout);

	va_end(_args);
}

void Seal_LogError(const char *fmt, Seal_Bool fatal, ...) {
	va_list _args;
	va_start(_args, fatal);
	
	printf("\033[31mERROR\033[0m: ");
	vprintf(fmt, _args);
	printf("\n");
	fflush(stdout);
	
	va_end(_args);

	if (fatal) exit(1);
}

void Seal_LogWarning(const char *fmt, ...) {
	va_list _args;
	va_start(_args, fmt);
	
	printf("\033[33mWARNING\033[0m: ");
	vprintf(fmt, _args);
	printf("\n");
	fflush(stdout);
	
	va_end(_args);
}

void *Seal_DeepCopy(void *src, size_t size) {
	void *dst = malloc(size);
	return memcpy(dst, src, size);
}