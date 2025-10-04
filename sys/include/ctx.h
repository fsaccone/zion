#ifndef _CTX_H
#define _CTX_H

#include <arch/ctx.h>
#include <arch/types.h>

/* Sets the program counter of context ctx to pc. */
void setctxpc(void *ctx, void *pc);

/* Sets the stack pointer of context ctx to sp. */
void setctxsp(void *ctx, void *sp);

/* Make CPU switch to the context in new after saving its current context in
   old. If old is NULL, it just loads new. */
void switchctx(void *old, void *new);

#endif
