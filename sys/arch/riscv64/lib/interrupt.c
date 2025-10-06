#include <interrupt.h>

#include <core.h>
#include <machine/cpu.h>

static void *handlers[NCPU] = { 0 };

void *
getinterrupthandler(void)
{
	return handlers[core()];
}

void
setinterrupthandler(void (*handler)(void))
{
	handlers[core()] = handler;
}
