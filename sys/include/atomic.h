#ifndef _ATOMIC_H
#define _ATOMIC_H

#include <arch/types.h>

/* It atomically swaps the value at d with s, returning the old value at d. */
ureg atomicswap(ureg *d, ureg s);

#endif
