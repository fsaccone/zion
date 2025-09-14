#include <panic.h>

#include <console.h>
#include <interrupt.h>
#include <pmem.h>
#include <spinlock.h>

static char *panicmsg = NULL;

static struct lock l = { 0 };

void
panic(void)
{
	char *c;

	disableinterrupts();

	/* This lock will never be freed. */
	acquirelock(&l);

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
