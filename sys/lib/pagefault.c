#include <pagefault.h>

#include <panic.h>

void
pagefault(void)
{
	setpanicmsg("Page fault.");
	tracepanicmsg("pagefault");
	panic();
}
