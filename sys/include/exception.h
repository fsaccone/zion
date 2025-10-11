#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include <arch/types.h>

/* The exception handler. The u argument is 1 when the exception is caused by
   code in user mode. */
void exception(u8 u);

#endif
