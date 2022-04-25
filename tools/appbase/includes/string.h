/**
 *Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 *Explorer C standard string head
 *Explorer 0.01/include/lib/string.h
 *version:Alpha
 *9/7/2014 2:19 PM
 */

#ifndef STRING_H_
#define STRING_H_

#include <stddef.h>

void *memset(void *s, int c, size_t count);
void *memcpy(void *dest, const void *src, size_t count);
void *memmove(void *dest, const void *src, size_t count);

// Get the address in string s1 when match string s2
char *strstr(const char *s1, const char *s2);

char *strchr(const char *string, int chr);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t count);
size_t strlcpy(char *dest, const char *src, size_t size);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *cs, const char *ct, size_t count);
size_t strlen(const char *s);

/**
 * strnlen - Find the length of a length-limited string
 * @s: The string to be sized
 * @count: The maximum number of bytes to search
 */
size_t strnlen(const char *s, size_t count);

size_t strspn(const char *s, const char *accept);

// Duplicate String
char *strndup(const char *src, size_t count);
char *strdup(const char *s);

#endif
