#include <arch/serial.h>

void
kmain(void)
{
	serial_print("Kernel start.\n");

	for (;;);
}
