#ifndef _ARCH_CTX_H
#define _ARCH_CTX_H

/* Registers are (1 ra + 1 sp + 1 gp + 1 tp + 12 * s = 16). Each register is 8
   bytes. */
#define CTX_SIZE (16 * 8)

#endif
