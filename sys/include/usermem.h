#ifndef _USERMEM_H
#define _USERMEM_H

#include <arch/types.h>

void  usermem_free(void *ptr);
void  usermem_freeall(void);
void *usermem_memset(void *s, int c, size_t n);

#endif
