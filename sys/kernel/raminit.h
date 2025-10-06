#ifndef __RAMINIT_H
#define __RAMINIT_H

#include <arch/types.h>
#include <pmem.h>

/* Sets frames linked list to the ordered list of frames containing the raminit
   program. Returns -1 in case of error or 0 otherwise. */
s8 raminitframes(struct framenode **frames);

#endif
