#ifndef _ATOMIC_H
#define _ATOMIC_H

#include <arch/types.h>

/* Atomically swaps the value at d with s and returns the old value. */
un atomicswap(un *d, un s);

#endif
