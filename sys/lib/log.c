#include <log.h>

#include <machine.h>

static void inttostr(char *str, long n, int base);
static void sprint(char *str);

static char digits[36] = "0123456789abcdefghijklmnopqrstuvwxyz";

static void
inttostr(char *str, long n, int base)
{
	int i = 0, j, l, neg = 0;

	if (!n) {
		str[i++] = '0';
		str[i++] = '\0';
		return;
	}

	if (n < 0) {
		neg = 1;
		n = -n;
	}

	for (; n; n /= base, i++) {
		int r = n % base;

		str[i] = digits[r % sizeof(digits)];
	}

	if (neg)
		str[i++] = '-';

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
	/*      DEBUG: */
	sprint("       ");
	sprint(m);
	sprint("\n");
}

void
debugheader(char *h)
{
	sprint("DEBUG: ");
	sprint(h);
	sprint("\n");
}

void
debugptr(void *ptr)
{
#ifdef BIT32
	char str[11];
#endif
#ifdef BIT64
	char str[19];
#endif
#ifdef BIT128
	char str[35];
#endif
	str[0] = '0';
	str[1] = 'x';

	inttostr(&str[2], (long)ptr, 16);

	/*      DEBUG: */
	sprint("       ");
	sprint(str);
	sprint("\n");
}

void
panic(char *m)
{
	sprint("PANIC: ");
	sprint(m);
	sprint("\n");

loop:
	goto loop;
}
