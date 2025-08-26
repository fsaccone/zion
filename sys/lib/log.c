#include <log.h>

#include <arch.h>

static void
sprint(const char *str)
{
	while (*str) {
		while(!(*UART_LSR & UART_LSR_EMPTY_MASK));
		*UART_THR = *str++;
	}
}

void
debug(const char *m)
{
	sprint("DEBUG: ");
	sprint(m);
	sprint("\n");
}

void
panic(const char *m)
{
	sprint("PANIC: ");
	sprint(m);
	sprint("\n");
	for (;;);
}
