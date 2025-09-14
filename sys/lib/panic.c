#include <panic.h>

#include <console.h>
#include <interrupt.h>
#include <pmem.h>

static char *panicmsg = NULL;

void
panic(void)
{
	char *c;

	disableinterrupts();

	consolewrite("[PANIC] ");

	for (c = panicmsg; *c; c++)
		consolewrite(c);

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
tracepanicmsg(char *t)
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

	while (*t)
		*new++ = *t++;

	*new++ = ')';
	*new++ = '\0';
}
