#include <interrupt.h>

#include "kernel.h"
#include "user.h"

void
interrupt(void)
{
	un *args = interruptargs();
	u8 isuser = interruptisuser();

	if (isuser)
		userinterrupt(args);
	else
		kernelinterrupt();
}
