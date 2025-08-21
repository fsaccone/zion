#include "trap.h"

#include <arch/types.h>

#include "exception.h"
#include "interrupt.h"

void
handle_trap(int32_t cause)
{
	if (cause < 0)
		handle_interrupt(cause + 16);
	else
		handle_exception(cause);
}
