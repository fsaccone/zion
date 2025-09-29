#include <string.h>

#include <arch/types.h>

s8
strncmp(char *s1, char *s2, uptr n)
{
	for (; n > 0; n--, s1++, s2++) {
		if (*s1 != *s2)
			return *(u8 *)s1 - *(u8 *)s2;

		if (!*s1)
			return 0;
	}

	return 0;
}
