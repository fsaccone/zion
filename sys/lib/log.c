#include <log.h>

#include <arch.h>
#include <machine.h>

static void inttostr(char *str, int n, int base);
static void sprint(char *str);

static void
inttostr(char *str, int n, int base)
{
	int i, j, l;

	if (!n) {
		str[0] = '0';
		str[1] = '\0';
		return;
	}

	for (i = 0; n; n /= base, i++) {
		int r = n % base;

		str[i] = (r > 9) ? ('a' + r - 10) : ('0' + r);
	}

	l = i;

	/* Reverse the string */
	for (j = 0; j < l / 2;j++) {
		char tmp = str[j];

		str[j] = str[l - j - 1];
		str[l - j - 1] = tmp;
	}

	str[l] = '\0';
}

static void
sprint(char *str)
{
	while (*str) {
		while(!(*UART_LSR & UART_LSR_EMPTY_MASK));
		*UART_THR = *str++;
	}
}

void
debug(char *m)
{
	sprint("DEBUG: ");
	sprint(m);
	sprint("\n");
}

void
panic(char *m)
{
	sprint("PANIC: ");
	sprint(m);
	sprint("\n");
	for (;;);
}
