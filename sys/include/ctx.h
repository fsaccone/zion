#ifndef _CTX_H
#define _CTX_H

#include <arch/ctx.h>
#include <arch/types.h>

/* Returns the program counter of context ctx. */
void *getctxpc(u8 ctx[CTX_SIZE]);

/* Sets the program counter of context ctx to pc. */
void setctxpc(u8 ctx[CTX_SIZE], void *pc);

/* Sets the stack pointer of context ctx to sp. */
void setctxsp(u8 ctx[CTX_SIZE], void *sp);

/* Make CPU switch to the context in new after saving its current context in
   old. If old is NULL, it just loads new. */
void switchctx(u8 old[CTX_SIZE], u8 new[CTX_SIZE]);

#endif
