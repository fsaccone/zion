#include <log.h>

#include <arch/bits.h>
#include <config.h>
#include <interrupt.h>
#include <machine/mem.h>
#include <pmem.h>

#define UART_THR            (UART0 + 0x00)
#define UART_LSR            (UART0 + 0x05)
#define UART_LSR_EMPTY_MASK (0x40)

/* Writes a NULL-terminated string to str which contains the number n written
   in base base. If sign is not 0, it treats n as signed and prefixes str with
   the - character if n is negative */
static void inttostr(char *str, u64 n, u8 base, u8 sign);

/* Writes NULL-terminated string str to UART */
static void sprint(char *str);

static char *panicmsg = NULL;

void
inttostr(char *str, u64 n, u8 base, u8 sign)
{
	int i = 0, j, l, neg = 0;
	char digits[36] = "0123456789abcdefghijklmnopqrstuvwxyz";

	if (!n) {
		str[i++] = '0';
		str[i++] = '\0';
		return;
	}

	if (sign && (s64)n < 0) {
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

void
sprint(char *str)
{
	u16 i;

	for (i = 0; i < LOG_MAX; i++) {
		if (!str[i])
			break;

		while(!(*UART_LSR & UART_LSR_EMPTY_MASK));

		*UART_THR = str[i];
	}
}

void
debug(char *m)
{
#ifdef CONFIG_DEBUG
	sprint(m);
#else
	(void)m;
#endif
}

void
debugintbase2(u64 n)
{
	/* 0b + 64 digits + \0  = 67 */
	char str[67];

	str[0] = '0';
	str[1] = 'b';

	inttostr(&str[2], n, 2, 0);

	debug(str);
}

void
debugintbase10s(s64 n)
{
	/* 20 digits + \0  = 21 */
	char str[21];

	inttostr(str, n, 10, 1);

	debug(str);
}

void
debugintbase10u(u64 n)
{
	/* 20 digits + \0  = 21 */
	char str[21];

	inttostr(str, n, 10, 0);

	debug(str);
}

void
debugintbase16(u64 n)
{
	/* 0x + 16 digits + \0 = 19 */
	char str[19];

	str[0] = '0';
	str[1] = 'x';

	inttostr(&str[2], n, 16, 0);

	debug(str);
}

void
panic(char *m)
{
	disableinterrupts();

	sprint("[PANIC] ");
	sprint(m);

	if (panicmsg) {
		sprint(": ");
		sprint(panicmsg);
	}

	sprint("\n");

loop:
	goto loop;
}

void
setpanicmsg(char *m)
{
	panicmsg = m;
}
