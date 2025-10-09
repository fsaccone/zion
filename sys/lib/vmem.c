#include <vmem.h>

#include <arch/page.h>
#include <arch/types.h>
#include <math.h>
#include <panic.h>
#include <pmem.h>
#include <trampoline.h>

pageentry *
allocpagetable(void)
{
	pageentry *ptable;

	if (!(ptable = palloc(PAGE_TABLE_ENTRIES * sizeof(pageentry), 0)))
		goto panic;

	return ptable;

panic:
	tracepanicmsg("allocpagetable");
	return NULL;
}

s8
allocvas(pageentry *ptree, void *tframe, u8 user)
{
	struct pageoptions popts = { 0 };
	uptr a;

	/* Trampoline. */
	popts.u = 0;
	popts.r = 1;
	popts.w = 0;
	popts.x = 1;
	if (vmap(ptree, VADDR_TRAMPOLINE, trampolinebase(), popts))
		goto panic;

	/* Trap frame. */
	popts.u = 0;
	popts.r = 1;
	popts.w = 1;
	popts.x = 0;
	if (vmap(ptree, VADDR_TRAP_FRAME, tframe, popts))
		goto panic;

	/* Stack (user-only). */
	if (!user)
		return 0;
	popts.u = 1;
	popts.r = 1;
	popts.w = 1;
	popts.x = 0;
	for (a = 0; a < STACK_SIZE; a += PAGE_SIZE) {
		void *f;

		if (!(f = palloc(PAGE_SIZE, 0)))
			goto panic;

		if (vmap(ptree, VADDR_STACK_START + a, f, popts))
			goto panic;
	}

	return 0;

panic:
	tracepanicmsg("allocvas");
	return -1;
}

s8
freepagetable(pageentry *ptable)
{
	if (pfree(ptable, PAGE_TABLE_ENTRIES * sizeof(pageentry)))
		goto panic;

	return 0;

panic:
	tracepanicmsg("freepagetable");
	return -1;
}

s8
freevasstack(pageentry *ptree)
{
	uptr a;

	for (a = 0; a < STACK_SIZE; a += PAGE_SIZE) {
		void *f;

		if (!(f = paddr(ptree, VADDR_STACK_START + a)))
			goto panic;

		if (pfree(f, PAGE_SIZE))
			goto panic;

		if (vunmap(ptree, VADDR_STACK_START + a))
			goto panic;
	}

	return 0;

panic:
	tracepanicmsg("freevasstack");
	return -1;
}

void *
paddr(pageentry *ptree, uptr vaddr)
{
	uptr l, lvlidxs[PAGE_TABLE_LEVELS] = PAGE_LVLIDXS_FROM_VADDR(vaddr);
	pageentry *lastpt, *e;

	/* Get last-level page table pointing to e starting from ptree. */
	lastpt = ptree;
	for (l = 0; l < PAGE_TABLE_LEVELS - 1; l++) {
		pageentry *lastpte = &lastpt[lvlidxs[l]];

		if (!PAGE_ENTRY_GET_VALID(*lastpte)) {
			setpanicmsg("Invalid page.");
			goto panic;
		} else if (!PAGE_ENTRY_GET_WALKABLE(*lastpte)) {
			setpanicmsg("Non-walkable page table.");
			goto panic;
		}

		lastpt = (pageentry *)PAGE_ENTRY_GET_PADDR(*lastpte);
	}

	e = &lastpt[lvlidxs[PAGE_TABLE_LEVELS - 1]];

	if (!PAGE_ENTRY_GET_VALID(*e)) {
		setpanicmsg("Invalid page.");
		goto panic;
	}

	return (void *)(PAGE_ENTRY_GET_PADDR(*e) + (vaddr % PAGE_SIZE));

panic:
	tracepanicmsg("paddr");
	return NULL;
}

s8
vmap(pageentry *ptree, uptr vaddr, void *paddr, struct pageoptions opts)
{
	uptr l, lvlidxs[PAGE_TABLE_LEVELS] = PAGE_LVLIDXS_FROM_VADDR(vaddr);
	pageentry *lastpt, *e;

	if (vaddr % PAGE_SIZE) {
		setpanicmsg("Unaligned virtual address.");
		goto panic;
	}

	if ((uptr)paddr % PAGE_SIZE) {
		setpanicmsg("Unaligned physical address.");
		goto panic;
	}

	if (!opts.r && !opts.w && !opts.x) {
		setpanicmsg("No permissions.");
		goto panic;
	}

	if (opts.w && !opts.r) {
		setpanicmsg("Write permission without read permission.");
		goto panic;
	}

	if (opts.x && !opts.r) {
		setpanicmsg("Execute permission without read permission.");
		goto panic;
	}

	/* Get last-level page table pointing to e starting from ptree. */
	lastpt = ptree;
	for (l = 0; l < PAGE_TABLE_LEVELS - 1; l++) {
		pageentry *lastpte = &lastpt[lvlidxs[l]];

		/* Allocate invalid intermediate page tables. */
		if (!PAGE_ENTRY_GET_VALID(*lastpte)) {
			pageentry *newpt;

			if (!(newpt = allocpagetable()))
				goto panic;

			*lastpte = PAGE_ENTRY_ADD_VALID(*lastpte);
			*lastpte = PAGE_ENTRY_SET_WALKABLE(*lastpte);
			*lastpte = PAGE_ENTRY_SET_PADDR(*lastpte, (uptr)newpt);
		} else if (!PAGE_ENTRY_GET_WALKABLE(*lastpte)) {
			setpanicmsg("Non-walkable page table.");
			goto panic;
		}

		lastpt = (pageentry *)PAGE_ENTRY_GET_PADDR(*lastpte);
	}

	e = &lastpt[lvlidxs[PAGE_TABLE_LEVELS - 1]];

	if (PAGE_ENTRY_GET_VALID(*e)) {
		setpanicmsg("Already valid page.");
		goto panic;
	}

	*e = PAGE_ENTRY_SET_PADDR(*e, (uptr)paddr);

	*e = PAGE_ENTRY_ADD_VALID(*e);

	if (opts.r)
		*e = PAGE_ENTRY_ADD_R(*e);

	if (opts.w)
		*e = PAGE_ENTRY_ADD_W(*e);

	if (opts.x)
		*e = PAGE_ENTRY_ADD_X(*e);

	if (opts.u)
		*e = PAGE_ENTRY_ADD_USER(*e);

	return 0;

panic:
	tracepanicmsg("vmap");
	return -1;
}

s8
vunmap(pageentry *ptree, uptr vaddr)
{
	uptr l, i, lvlidxs[PAGE_TABLE_LEVELS] = PAGE_LVLIDXS_FROM_VADDR(vaddr);
	pageentry *lastpt, *e;

	if (vaddr % PAGE_SIZE) {
		setpanicmsg("Unaligned virtual address.");
		goto panic;
	}

	/* Get last-level page table pointing to e starting from ptree. */
	lastpt = ptree;
	for (l = 0; l < PAGE_TABLE_LEVELS - 1; l++) {
		pageentry *lastpte = &lastpt[lvlidxs[l]];

		if (!PAGE_ENTRY_GET_VALID(*lastpte)) {
			setpanicmsg("Invalid page.");
			goto panic;
		} else if (!PAGE_ENTRY_GET_WALKABLE(*lastpte)) {
			setpanicmsg("Non-walkable page table.");
			goto panic;
		}

		lastpt = (pageentry *)PAGE_ENTRY_GET_PADDR(*lastpte);
	}

	e = &lastpt[lvlidxs[PAGE_TABLE_LEVELS - 1]];

	if (!PAGE_ENTRY_GET_VALID(*e)) {
		setpanicmsg("Invalid page.");
		goto panic;
	}

	*e = PAGE_ENTRY_REM_VALID(*e);

	/* Check if any page table is empty (i.e. full of invalid entries): if
	   it is, free it. */
	for (i = 0; i < PAGE_TABLE_LEVELS - 1; i++) {
		pageentry *pt, *etopt;
		uptr j, ei;
		u8 isempty;

		/* Find current page table pt and the entry pointing to it,
		   etopt, which is needed when pt has to be freed. */
		pt = ptree;
		for (j = 0; j < PAGE_TABLE_LEVELS - i - 1; j++) {
			/* Set etopt on the previous pt, which is the parent of
			   the pt which is going to be set next. */
			etopt = &pt[lvlidxs[j]];

			pt = (pageentry *)PAGE_ENTRY_GET_PADDR(pt[lvlidxs[j]]);
		}

		/* Set isempty. */
		isempty = 1;
		for (ei = 0; ei < PAGE_TABLE_ENTRIES && isempty; ei++) {
			if (PAGE_ENTRY_GET_VALID(pt[ei]))
				isempty = 0;
		}

		if (isempty) {
			if (freepagetable(pt))
				goto panic;

			*etopt = PAGE_ENTRY_REM_VALID(*etopt);
		}
	}

	return 0;

panic:
	tracepanicmsg("vunmap");
	return -1;
}
