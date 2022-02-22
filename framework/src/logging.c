
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <seal/common.h>

static void _LogWithArgs(const char *fmt, va_list vargs) {
	vprintf(fmt, vargs);
	printf("\n");
	fflush(stdout);

	va_end(vargs);
}

void Seal_Log(const char *fmt, ...) {
	va_list _args;
	va_start(_args, fmt);
	
	_LogWithArgs(fmt, _args);
}

void Seal_LogError(const char *fmt, ...) {
	va_list _args;
	va_start(_args, fmt);
	
	printf("[\033[31mERROR\033[0m]: ");
	_LogWithArgs(fmt, _args);
}

void Seal_LogInfo(const char *fmt, ...) {
	va_list _args;
	va_start(_args, fmt);
	
	printf("[\033[34;1mINFO\033[0m]: ");
	_LogWithArgs(fmt, _args);
}

void Seal_LogWarning(const char *fmt, ...) {
	va_list _args;
	va_start(_args, fmt);
	
	printf("[\033[33mWARNING\033[0m]: ");
	_LogWithArgs(fmt, _args);
}

void Seal_LogFatal(const char *fmt, ...) {
	va_list _args;
	va_start(_args, fmt);

	printf("[\033[31mFATAL\033[0m]: ");
	_LogWithArgs(fmt, _args);
	exit(1);
}