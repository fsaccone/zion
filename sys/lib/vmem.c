#include <vmem.h>

#include <arch/page.h>
#include <arch/types.h>
#include <math.h>
#include <panic.h>
#include <pmem.h>

pageentry *
allocpagetable(void)
{
	pageentry *ptable;

	if (!(ptable = palloc(PAGE_TABLE_ENTRIES * sizeof(pageentry), 0))) {
		tracepanicmsg("allocpagetable");
		return NULL;
	}

	return ptable;
}

s8
valloc(pageentry ptree[PAGE_TABLE_ENTRIES], uptr vaddr,
       struct pageoptions opts)
{
	uptr l, lvlidxs[PAGE_TABLE_LEVELS] = PAGE_LVLIDXS_FROM_VADDR(vaddr);
	pageentry *lastpt, *e;
	void *f;

	/* Get last-level page table pointing to e starting from ptree. */
	lastpt = ptree;
	for (l = 0; l < PAGE_TABLE_LEVELS - 1; l++) {
		pageentry *lastpte = &lastpt[lvlidxs[l]];

		/* Allocate invalid intermediate page tables. */
		if (!PAGE_ENTRY_GET_VALID(*lastpte)) {
			pageentry *newpt;

			if (!(newpt = allocpagetable())) {
				tracepanicmsg("valloc");
				return -1;
			}

			*lastpte = PAGE_ENTRY_ADD_VALID(*lastpte);
			*lastpte = PAGE_ENTRY_SET_WALKABLE(*lastpte);
			*lastpte = PAGE_ENTRY_SET_PADDR(*lastpte, (uptr)newpt);
		} else if (!PAGE_ENTRY_GET_WALKABLE(*lastpte)) {
			setpanicmsg("Non-walkable page table.");
			tracepanicmsg("valloc");
			return -1;
		}

		lastpt = (pageentry *)PAGE_ENTRY_GET_PADDR(*lastpte);
	}

	e = &lastpt[lvlidxs[PAGE_TABLE_LEVELS - 1]];

	if (PAGE_ENTRY_GET_VALID(*e)) {
		setpanicmsg("Already valid page.");
		tracepanicmsg("valloc");
		return -1;
	}

	if (!(f = palloc(PAGE_SIZE, 0))) {
		tracepanicmsg("valloc");
		return -1;
	}

	*e = PAGE_ENTRY_SET_PADDR(*e, (uptr)f);

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
}

s8
vfree(pageentry ptree[PAGE_TABLE_ENTRIES], uptr vaddr)
{
	uptr l, i, lvlidxs[PAGE_TABLE_LEVELS] = PAGE_LVLIDXS_FROM_VADDR(vaddr);
	pageentry *lastpt, *e;
	void *f;

	/* Get last-level page table pointing to e starting from ptree. */
	lastpt = ptree;
	for (l = 0; l < PAGE_TABLE_LEVELS; l++) {
		pageentry *lastpte = &lastpt[lvlidxs[l]];

		if (!PAGE_ENTRY_GET_VALID(*lastpte)) {
			setpanicmsg("Invalid page.");
			tracepanicmsg("vfree");
			return -1;
		} else if (!PAGE_ENTRY_GET_WALKABLE(*lastpte)) {
			setpanicmsg("Non-walkable page table.");
			tracepanicmsg("vfree");
			return -1;
		}

		lastpt = (pageentry *)PAGE_ENTRY_GET_PADDR(*lastpte);
	}

	e = &lastpt[lvlidxs[PAGE_TABLE_LEVELS - 1]];

	if (!PAGE_ENTRY_GET_VALID(*e)) {
		setpanicmsg("Invalid page.");
		tracepanicmsg("vfree");
		return -1;
	}

	f = (void *)PAGE_ENTRY_GET_PADDR(*e);

	if (pfree(f, PAGE_SIZE)) {
		tracepanicmsg("vfree");
		return -1;
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
			/* Free and invalidate page table. */
			if (pfree(pt,
			          sizeof(pageentry) * PAGE_TABLE_ENTRIES)) {
				tracepanicmsg("vfree");
				return -1;
			}

			*etopt = PAGE_ENTRY_REM_VALID(*etopt);
		}
	}

	return 0;
}
