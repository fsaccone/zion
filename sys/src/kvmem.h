#ifndef __KVMEM_H
#define __KVMEM_H

#include <arch/page.h>
#include <arch/types.h>

/* It allocates a page tree and initializes it with the kernel virtual address
   space. On success, 0 is returned and ptree is set to the address of the
   allocated page tree. On failure, -1 is returned and the panic message is
   set. */
s8 kvmem(pageentry **ptree);

#endif
