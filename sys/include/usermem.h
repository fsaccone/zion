#ifndef _USERMEM_H
#define _USERMEM_H

#include <arch/types.h>

void *usermem_alloc(size_t s);
void  usermem_free(void *ptr);
void  usermem_init(void *kstart);
void *usermem_memset(void *s, int c, size_t n);

#endif
