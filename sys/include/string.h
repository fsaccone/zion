#ifndef _STRING_H
#define _STRING_H

#include <arch/types.h>

/* Returns 0 if s1 is equal to s2, a negative number if s1 is less than s2 or a
   positive number if s1 is greater than s2. The maximum characters to be
   compared is n. */
u8 strncmp(char *s1, char *s2, uptr n);

#endif
