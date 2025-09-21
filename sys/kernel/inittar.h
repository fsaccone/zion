#ifndef __INITTAR_H
#define __INITTAR_H

#include <inittar.h>
#include <tar.h>

/* Returns the init.tar archive starting address contained inside the kernel
   codespace, or NULL if it is not found. */
struct tarheader *inittaraddress(void);

#endif
