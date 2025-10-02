#ifndef _CTX_H
#define _CTX_H

#include <arch/ctx.h>
#include <arch/types.h>

/* Make CPU switch to the context in new after saving its current context in
   old. If old is NULL, it just loads new. */
void switchctx(u8 old[CTX_SIZE], u8 new[CTX_SIZE]);

#endif
