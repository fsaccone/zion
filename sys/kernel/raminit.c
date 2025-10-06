#include "raminit.h"

#include <config.h>
#include <machine/mem.h>

void *
raminitbase(void)
{
	return (void *)(KERNEL_START + BINARY_SIZE);
}
