#ifndef _CTX_H
#define _CTX_H

/* Make CPU switch to the context in new after saving its current context in
   old. If old is NULL, it just loads new */
void switchctx(void *old, void *new);

#endif
