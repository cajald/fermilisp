/*
 * util.h -- utilities
 *
 * Mostly for safety
 */

#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

extern char* argv0;

void  die(const char* msg);
void* emalloc(size_t size);
void* ecalloc(size_t nmemb, size_t size);
void* erealloc(void* ptr, size_t newsize);
void  efree(void* ptr);

#define ASSERT(x)	do { if (!(x)) { \
				fprintf(stderr, "%s: failed assertion at %s:%d\n", \
					argv0, __FILE__,__LINE__); \
				abort(); \
			} } while (0)

#define ARRSIZE(arr)	sizeof(arr) / arr[0];

#endif /* UTILS_H */
