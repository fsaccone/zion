/* ARCHITECTURE-SPECIFIC */
#ifndef _ATOMIC_H
#define _ATOMIC_H

#include <arch.h>

/* Atomically swaps the value at d with s and returns the old value */
uintn atomicswap(uintn *d, uintn s);

#endif
