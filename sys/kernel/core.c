#include "core.h"

#include <arch/types.h>
#include <timer.h>

void
coremain(u16 c)
{
	(void)c;

	setupnexttimer();

	for (;;);
}
