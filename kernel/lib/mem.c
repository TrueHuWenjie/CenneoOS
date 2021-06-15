/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer C library memory
 * Explorer/lib/mm.c
 * version:Alpha
 * 11/8/2014 7:37 PM
 */

#include <stddef.h>
#include <lib/mem.h>

// Find a value in memory field
void *memchr(void * ptr, int value, size_t num)
{

}

/**
 * memset - Fill a region of memory with the given value
 * @buffer: Pointer to the start of the area.
 * @c: The byte to fill the area with
 * @count: The size of the area.
 *
 * Do not use memset() to access IO space, use memset_io() instead.
 */
void *memset(void *buffer, int c, size_t count)
{
	char *xs = buffer;

	while (count--)
		*xs++ = c;
	return buffer;
}

/**
 * memcpy - Copy one area of memory to another
 * @dest: Where to copy to
 * @src: Where to copy from
 * @count: The size of the area.
 *
 * You should not use this function to access IO space, use memcpy_toio()
 * or memcpy_fromio() instead.
 */
void *memcpy(void *dest, const void *src, size_t count)
{
	char *dest_c;
	const char *src_c;
	long *dest_l = dest;
	const long *src_l = src;

	size_t count_l = count / sizeof(long);
	count = count % sizeof(long);

	while (count_l --)
		*dest_l ++ = *src_l ++;  

	dest_c = (char *)dest_l;
	src_c = (const char *) src_l;

    while (count   --)
		*dest_c ++ = *src_c ++;
	return dest;
}

/**
 * memmove - Copy one area of memory to another
 * @dest: Where to copy to
 * @src: Where to copy from
 * @count: The size of the area.
 *
 * Unlike memcpy(), memmove() copes with overlapping areas.
 */
void *memmove(void *dest, const void *src, size_t count)
{
	char *tmp;
	const char *s;

	if (dest <= src) {
		tmp = dest;
		s = src;
		while (count--)
			*tmp ++ = *s ++;
	} else {
		tmp = dest;
		tmp += count;
		s = src;
		s += count;
		while (count--)
			*--tmp = *--s;
	}
	return dest;
}
