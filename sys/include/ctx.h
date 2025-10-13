#ifndef _CTX_H
#define _CTX_H

#include <arch/ctx.h>
#include <arch/types.h>

/* It sets the program counter of context ctx to address pc. */
void setctxpc(u8 ctx[CTX_SIZE], void *pc);

/* It makes the calling core switch to the context in new after saving its
   current context to old. */
void switchctx(u8 old[CTX_SIZE], u8 new[CTX_SIZE]);

#endif
