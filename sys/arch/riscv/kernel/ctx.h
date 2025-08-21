#ifndef __CTX_H
#define __CTX_H

#include <arch/types.h>

void switch_ctx(uint32_t old[31], uint32_t new[31]);

#endif
