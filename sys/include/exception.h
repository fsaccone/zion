#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include <arch/types.h>

/* It handles a kernel exception if u is 0 or a user exception otherwise. */
void exception(u8 u);

#endif
