#include "trap.h"

#include <types.h>

#include "exception.h"
#include "interrupt.h"

void
handletrap(sn cause)
{
	if (cause < 0)
		handleinterrupt(cause + 16);
	else
		handleexception(cause);
}
