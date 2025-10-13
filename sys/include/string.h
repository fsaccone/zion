#ifndef _STRING_H
#define _STRING_H

#include <arch/types.h>

/* If s1 is equal to s2, 0 is returned. If s1 is less than s2, a negative
   number is returned. If s1 is greater than s2, a positive number is returned.
   It compares only the first n bytes of s1 and s2. */
s8 strncmp(char *s1, char *s2, uptr n);

#endif
