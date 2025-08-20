#include <arch/uart.h>

void
kmain(void)
{
	uart_print("Kernel start.\n");

	for (;;);
}
