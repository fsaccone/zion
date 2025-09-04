#include <process.h>

#include <log.h>
#include <vmem.h>

static struct process init = { 0 };
static int initdone = 0;

struct process *
initprocess(void)
{
	if (initdone)
		panic("initprocess: Called for the second time.");

	init.pid = 0;
	init.state = CREATED;
	init.pagetable = createpagetable();

	initdone = 1;

	return &init;
}
