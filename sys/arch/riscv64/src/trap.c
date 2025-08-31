#include "trap.h"

#include <arch.h>

#include "exception.h"
#include "interrupt.h"

void
handletrap(intn cause)
{
	if (cause < 0)
		handleinterrupt(cause + 16);
	else
		handleexception(cause);
}
