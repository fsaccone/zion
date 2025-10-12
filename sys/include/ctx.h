#ifndef _CTX_H
#define _CTX_H

#include <arch/ctx.h>
#include <arch/types.h>

/* Sets the program counter of context ctx to pc. */
void setctxpc(u8 ctx[CTX_SIZE], void *pc);

/* Make CPU switch to the context in new after saving its current context in
   old. */
void switchctx(u8 old[CTX_SIZE], u8 new[CTX_SIZE]);

#endif
