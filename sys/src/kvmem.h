#ifndef __KVMEM_H
#define __KVMEM_H

#include <arch/page.h>
#include <arch/types.h>

/* It allocates a page tree and initializes it with the kernel virtual address
   space. On success, a pointer to the allocated page tree is returned. On
   failure, NULL is returned and the panic message is set. */
pageentry *kvmem(void);

#endif
