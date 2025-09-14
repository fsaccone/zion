#include <panic.h>

#include <console.h>
#include <interrupt.h>
#include <pmem.h>
#include <spinlock.h>

static char        panicmsg[CONSOLE_WRITE_MAX] = { 0 };
static struct lock l                           = { 0 };

void
panic(void)
{
	disableinterrupts();

	/* This lock will never be freed. */
	acquirelock(&l);

	consolewrite("[PANIC] ");

	consolewrite(panicmsg);

	consolewrite("\n");

loop:
	goto loop;
}

void
setpanicmsg(char *m)
{
	u32 i;

	for (i = 0; m[i] && i < CONSOLE_WRITE_MAX; i++)
		panicmsg[i] = m[i];
}

void
tracepanicmsg(char *t)
{
	u32 i, j;
	char pre[5]  = " (<- ",
	     post[1] = ")";

	/* Get to the end of panicmsg. */
	for (i = 0; panicmsg[i] && i < CONSOLE_WRITE_MAX; i++);

	/* Write pre. */
	for (j = 0; j < sizeof(pre) && i < CONSOLE_WRITE_MAX; j++, i++)
		panicmsg[i] = pre[j];

	/* Write t. */
	for (j = 0; t[j] && i < CONSOLE_WRITE_MAX; j++, i++)
		panicmsg[i] = t[j];

	/* Write post. */
	for (j = 0; j < sizeof(post) && i < CONSOLE_WRITE_MAX; j++, i++)
		panicmsg[i] = post[j];
}
