#include <panic.h>

#include <console.h>
#include <interrupt.h>
#include <pmem.h>

static char *panicmsg = NULL;

void
panic(char *m)
{
	disableinterrupts();

	consolewrite("[PANIC] ");

	for (; *m; m++)
		consolewrite(m);

	if (panicmsg) {
		char *c;

		consolewrite(": ");

		for (c = panicmsg; *c; c++)
			consolewrite(c);
	}

	consolewrite("\n");

loop:
	goto loop;
}

void
setpanicmsg(char *m)
{
	panicmsg = m;
}

void
tracepanicmsg(char *m)
{
	char *new = panicmsg;

	/* Get to the end of panicmsg. */
	while (*new)
		new++;

	/* (" (<- ") */
	*new++ = ' ';
	*new++ = '(';
	*new++ = '<';
	*new++ = '-';
	*new++ = ' ';

	while (*m)
		*new++ = *m++;

	*new++ = ')';
	*new++ = '\0';
}
