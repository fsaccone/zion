#include <process.h>

#include <log.h>
#include <vmem.h>

static struct process init     = { 0 };
static int            initdone = 0;

static u8             pidbitmap[PID_MAX / 8] = { 0 };

/* Returns the first unused PID from pidbitmap and sets it to used. Returns 0
   if pidbitmap is full */
static u16 unusedpid(void);

u16
unusedpid(void)
{
	u16 i;

	for (i = 0; i < PID_MAX; i++) {
		if (!(pidbitmap[i / 8] & (1 << (i % 8)))) {
			pidbitmap[i / 8] |= (1 << (i % 8));
			return i;
		}
	}

	return 0;
}

struct process *
initprocess(void)
{
	if (initdone)
		panic("initprocess: Called for the second time.");

	/* Set bit 0 to used */
	pidbitmap[0] |= 1;

	init.pid = 0;
	init.state = CREATED;
	init.pagetable = createpagetable();
	init.children = NULL;

	initdone = 1;

	return &init;
}
