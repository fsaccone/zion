#include <arch/types.h>
#include <interrupt.h>
#include <log.h>
#include <syscall.h>

/* type - The interrupt type code.
   args - The array of arguments passed to the system call. Always NULL if the
          interrupt type is not syscall.
*/
void
interrupt(u8 type, un args[SYSCALL_ARGS_LEN])
{
	(void)args;

	switch (type) {
	case INTERRUPT_TYPE_EXCEPTION:
		setpanicmsg("Exception.");
		break;
	case INTERRUPT_TYPE_HARDWARE:
		setpanicmsg("Hardware.");
		break;
	case INTERRUPT_TYPE_SOFTWARE:
		setpanicmsg("Software.");
		break;
	case INTERRUPT_TYPE_TIMER:
		setpanicmsg("Timer.");
		break;
	case INTERRUPT_TYPE_SYSCALL:
		setpanicmsg("Syscall.");
		break;
	case INTERRUPT_TYPE_UNKNOWN:
	default:
		setpanicmsg("Unknown.");
	}

	panic("interrupt");
}
