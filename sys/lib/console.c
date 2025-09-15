#include <console.h>

#include <arch/types.h>
#include <driver.h>
#include <machine/drivers.h>
#include <spinlock.h>

/* Writes a NULL-terminated string to str which contains the number n written
   in base base. If sign is not 0, it treats n as signed and prefixes str with
   the - character if n is negative. */
static void inttostr(char *str, u64 n, u8 base, u8 sign);

static struct lock l = { 0 };

void
inttostr(char *str, u64 n, u8 base, u8 sign)
{
	u16 i = 0, j, l;
	u8 neg = 0;
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
		u8 r = n % base;

		str[i] = digits[r % sizeof(digits)];
	}

	if (neg)
		str[i++] = '-';

	l = i;

	/* Reverse the string. */
	for (j = 0; j < l / 2;j++) {
		char tmp = str[j];

		str[j] = str[l - j - 1];
		str[l - j - 1] = tmp;
	}

	str[l] = '\0';
}

u16
consolewrite(char *m)
{
#ifdef DRIVER_UART
	u16 i;

	lock(&l);

	for (i = 0; *m && i < CONSOLE_WRITE_MAX; m++, i++)
		driver_uart().write.character(*m);

	unlock(&l);

	return i;
#else
	(void)m;

	return 0;
#endif
}

u16
consolewriteb2(u64 n)
{
	/* (0b + 64 digits + \0  = 67) */
	char str[67];

	str[0] = '0';
	str[1] = 'b';

	inttostr(&str[2], n, 2, 0);

	return consolewrite(str);
}

u16
consolewriteb10s(s64 n)
{
	/* (20 digits + \0  = 21) */
	char str[21];

	inttostr(str, n, 10, 1);

	return consolewrite(str);
}

u16
consolewriteb10u(u64 n)
{
	/* (20 digits + \0  = 21) */
	char str[21];

	inttostr(str, n, 10, 0);

	return consolewrite(str);
}

u16
consolewriteb16(u64 n)
{
	/* (0x + 16 digits + \0 = 19) */
	char str[19];

	str[0] = '0';
	str[1] = 'x';

	inttostr(&str[2], n, 16, 0);

	return consolewrite(str);
}

void
initconsole(void)
{
#ifdef DRIVER_UART
	driver_uart().init();
#endif
}
