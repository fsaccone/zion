#include <log.h>

#include <arch/bits.h>
#include <machine/mem.h>

#define UART_THR            (UART0 + 0x00)
#define UART_LSR            (UART0 + 0x05)
#define UART_LSR_EMPTY_MASK (0x40)

/* Writes a NULL-terminated string to str which contains the number n written
   in base base */
static void inttostr(char *str, s64 n, u8 base);

/* Writes NULL-terminated string str to UART */
static void sprint(char *str);

static void
inttostr(char *str, s64 n, u8 base)
{
	int i = 0, j, l, neg = 0;
	char digits[36] = "0123456789abcdefghijklmnopqrstuvwxyz";

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
debugintbase2(u64 n)
{
	/* 0b + 64 digits + \0  = 67 */
	char str[67];

	str[0] = '0';
	str[1] = 'b';

	inttostr(&str[2], n, 2);

	/*      DEBUG: */
	sprint("       ");
	sprint(str);
	sprint("\n");
}

void
debugintbase10(u64 n)
{
	/* 20 digits + \0  = 21 */
	char str[21];

	inttostr(str, n, 10);

	/*      DEBUG: */
	sprint("       ");
	sprint(str);
	sprint("\n");
}

void
debugintbase16(u64 n)
{
	/* 0x + 16 digits + \0 = 19 */
	char str[19];

	str[0] = '0';
	str[1] = 'x';

	inttostr(&str[2], n, 16);

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
