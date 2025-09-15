#include <panic.h>

#include <console.h>
#include <core.h>
#include <machine/cpu.h>
#include <pmem.h>
#include <spinlock.h>

static char        panicmsg[NCPU][CONSOLE_WRITE_MAX] = { 0 };
static struct lock l                                 = { 0 };

void
panic(void)
{
	/* This lock will never be released. It also already disables interrupt
	   and that means it is not needed to do it manually. */
	lock(&l);

	(void)consolewrite("[PANIC] ");

	(void)consolewrite(panicmsg[core()]);

	(void)consolewrite("\n");

loop:
	goto loop;
}

void
setpanicmsg(char *m)
{
	u32 i;

	for (i = 0; m[i] && i < CONSOLE_WRITE_MAX; i++)
		panicmsg[core()][i] = m[i];
}

void
tracepanicmsg(char *t)
{
	u32 i, j;
	char pre[5]  = " (<- ",
	     post[1] = ")";

	/* Get to the end of panicmsg. */
	for (i = 0; panicmsg[core()][i] && i < CONSOLE_WRITE_MAX; i++);

	/* Write pre. */
	for (j = 0; j < sizeof(pre) && i < CONSOLE_WRITE_MAX; j++, i++)
		panicmsg[core()][i] = pre[j];

	/* Write t. */
	for (j = 0; t[j] && i < CONSOLE_WRITE_MAX; j++, i++)
		panicmsg[core()][i] = t[j];

	/* Write post. */
	for (j = 0; j < sizeof(post) && i < CONSOLE_WRITE_MAX; j++, i++)
		panicmsg[core()][i] = post[j];
}
