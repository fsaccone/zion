#include <log.h>

#include <arch.h>

static void
serial_print(const char *str)
{
	while (*str) {
		while(!(*UART_LSR & UART_LSR_EMPTY_MASK));
		*UART_THR = *str++;
	}
}

void
debug(const char *m)
{
	serial_print("DEBUG: ");
	serial_print(m);
	serial_print("\n");
}

void
panic(const char *m)
{
	serial_print("PANIC: ");
	serial_print(m);
	serial_print("\n");
	for (;;);
}
