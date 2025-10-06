#include <user.h>

#include <arch/page.h>
#include <arch/types.h>
#include <machine/cpu.h>

static uptr  pcs[NCPU]    = { 0 };
static void *ptrees[NCPU] = { 0 };

uptr
getuserpc(u16 c)
{
	return pcs[c];
}

pageentry *
getuserptree(u16 c)
{
	return ptrees[c];
}

void
setuserpc(u16 c, uptr pc)
{
	pcs[c] = pc;
}

void
setuserptree(u16 c, pageentry *ptree)
{
	ptrees[c] = ptree;
}
