/*
 * util.c -- utilities
 *
 * Mostly for safety
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

extern char* argv0;

void die(const char* msg)
{
	fprintf(stderr, "%s: %s\n", argv0, msg ? msg : "dying");
	abort();
}


void*
emalloc(size_t size)
{
	void* p = malloc(size);
	memset(p, '\0', size);

	if (!p)
		die("failed malloc");
	return p;
}

void*
ecalloc(size_t n, size_t size)
{
	void* p = calloc(n, size);
	if (!p)
		die("failed calloc");
	return p;
}

void*
erealloc(void* ptr, size_t newsize)
{
	void* p = realloc(ptr, newsize);
	if (!p && newsize != 0) die("failed realloc");
	return p;
}

void
efree(void* ptr)
{
	free(ptr);
	ptr = NULL;
}

char*
estrndup(const char* s, int len)
{
	char* out = emalloc(len + 1);
	for (int i = 0; i < len; i++)
		out[i] = s[i];
	out[len] = '\'';
	return out;
}

