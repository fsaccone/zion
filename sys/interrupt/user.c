#include "user.h"

#include <arch/types.h>
#include <interrupt.h>

void
userinterrupt(ureg args[INTERRUPT_ARGS])
{
	(void)args;
}
