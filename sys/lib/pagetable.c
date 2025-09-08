#include <pagetable.h>

#include <arch/page.h>
#include <arch/types.h>
#include <log.h>
#include <pmem.h>

/* Returns the first valid entry in page tree pt which points to address ptr,
   or NULL if ptr is not found */
static pageentry *findpointerentry(pageentry *pt[PAGE_TABLE_ENTRIES],
                                   void *ptr);

/* Returns the first invalid entry in page table pt, or NULL if pt is full */
static pageentry *invalidentry(pageentry *pt[PAGE_TABLE_ENTRIES]);

/* Returns the first non-full l-level page table it encounters in page tree pt,
   or NULL if pt is full. It creates one if needed */
static pageentry **levelpagetable(pageentry *pt[PAGE_TABLE_ENTRIES], u8 l);

/* Makes tables a NULL terminated array containing, in order from root to close
   parent, the path of tables eventually pointing to page table entry pte.
   tables[0] is set to NULL if pte is not found */
static void parenttables(pageentry **tables[PAGE_TABLE_LEVELS + 1],
                         pageentry *pt[PAGE_TABLE_ENTRIES],
                         pageentry *pte);

/* Returns the first valid entry in page table pt, or NULL if pt is empty */
static pageentry *validentry(pageentry *pt[PAGE_TABLE_ENTRIES]);

pageentry *
findpointerentry(pageentry *pt[PAGE_TABLE_ENTRIES], void *ptr)
{
	pageentry **curpt = pt;
	int i;

redowalk:
	for (i = 0; i < PAGE_TABLE_ENTRIES; i++) {
		pageentry ppn, *pte;

		pte = (pageentry *)((un)curpt + i * sizeof(pageentry *));
		ppn = PAGE_ENTRY_GET_PPN(*pte);

		if (ppn == (pageentry)ptr)
			return pte;

		if (PAGE_ENTRY_GET_WALKABLE(*pte)) {
			curpt = (pageentry **)ppn;

			goto redowalk;
		}
	}

	return NULL;
}

pageentry *
invalidentry(pageentry *pt[PAGE_TABLE_ENTRIES])
{
	int i;

	for (i = 0; i < PAGE_TABLE_ENTRIES; i++) {
		pageentry *pte = (void *)((un)pt + i * sizeof(pageentry *));

		if (!PAGE_ENTRY_GET_VALID(*pte))
			return pte;
	}

	return NULL;
}

pageentry **
levelpagetable(pageentry *pt[PAGE_TABLE_ENTRIES], u8 l)
{
	int i, lvlidx[PAGE_TABLE_LEVELS] = { 0 };
	pageentry **lastpt;

	if (l < 1 && l > PAGE_TABLE_LEVELS) {
		setpanicmsg("Passed non-existent level.");
		tracepanicmsg("levelpagetable");
		return NULL;
	}

	lastpt = pt;
	for (i = 0; i < l - 1; i++) {
		/* We need to keep track of the last entry index of each level
		   in case we need to go backwards after walking through a
		   full table: lvlidx does that */
		for (; lvlidx[i] < PAGE_TABLE_ENTRIES; lvlidx[i]++) {
			pageentry *pte;

			pte = (pageentry *)((un)lastpt
			                    + lvlidx[i] * sizeof(pageentry *));

			/* If invalid, create and walk in a new pt */
			if (!PAGE_ENTRY_GET_VALID(*pte)) {
				lastpt = allocpagetable();

				*pte = PAGE_ENTRY_SET_PPN(*pte,
				                          (pageentry)lastpt);
				*pte = PAGE_ENTRY_ADD_VALID(*pte);
				*pte = PAGE_ENTRY_SET_WALKABLE(*pte);

				goto nextlevel;
			}

			if (PAGE_ENTRY_GET_WALKABLE(*pte)) {
				un addr = PAGE_ENTRY_GET_PPN(*pte);

				lastpt = (pageentry **)addr;

				goto nextlevel;
			}
		}

walkback:
		/* If we walked through the whole root page table, then it is
		   full */
		if (lvlidx[0] >= PAGE_TABLE_ENTRIES - 1)
			return NULL;

		/* If none of the entries are walkable, there is no space in
		   lastpt: we need to go back one level and continue from
		   where we left */
		i -= 2;
		continue;

nextlevel:
		if (i < l - 1)
			continue;

		/* Last iteration only */

		/* If lastpt is full, walk back and continue */
		if (!invalidentry(lastpt))
			goto walkback;
	}

	return lastpt;
}

void
parenttables(pageentry **tables[PAGE_TABLE_LEVELS + 1],
             pageentry *pt[PAGE_TABLE_ENTRIES],
             pageentry *pte)
{
	int i, lvlidx[PAGE_TABLE_LEVELS] = { 0 };

	/* Walk the whole tree saving each level to tables until pte is
	   found */
	tables[0] = pt;
	for (i = 0; i < PAGE_TABLE_LEVELS; i++) {
		for (; lvlidx[i] < PAGE_TABLE_ENTRIES; lvlidx[i]++) {
			pageentry *e;

			e = (pageentry *)((un)tables[i]
			                  + lvlidx[i] * sizeof(pageentry *));

			if (!PAGE_ENTRY_GET_VALID(*e))
				continue;

			if ((un)e == (un)pte) {
				tables[i + 1] = NULL;
				return;
			}

			if (PAGE_ENTRY_GET_WALKABLE(*e)) {
				pageentry addr = PAGE_ENTRY_GET_PPN(*e);

				tables[i + 1] = (pageentry **)addr;

				goto nextlevel;
			}
		}

		/* If we walked through the whole root page table, then pte was
		   not found */
		if (lvlidx[0] >= PAGE_TABLE_ENTRIES - 1)
			break;

		/* If we walked through a table full of non-walkable entries,
		   walk back */
		i -= 2;

nextlevel:
	}

	/* If no return, then pte was not found */
	tables[0] = NULL;
}

pageentry *
validentry(pageentry *pt[PAGE_TABLE_ENTRIES])
{
	int i;

	for (i = 0; i < PAGE_TABLE_ENTRIES; i++) {
		pageentry *pte = (void *)((un)pt + i * sizeof(pageentry *));

		if (PAGE_ENTRY_GET_VALID(*pte))
			return pte;
	}

	return NULL;
}

pageentry *
allocpage(pageentry *pt[PAGE_TABLE_ENTRIES], struct pageoptions opts)
{
	pageentry **lastpt, *pte;
	void *f;

	if (!opts.r && !opts.w && !opts.x) {
		setpanicmsg("No permissions passed.");
		tracepanicmsg("allocpage");
		return NULL;
	}

	if (opts.x && !opts.r) {
		setpanicmsg("Execute permission passed without read one.");
		tracepanicmsg("allocpage");
		return NULL;
	}

	if (!(lastpt = levelpagetable(pt, PAGE_TABLE_LEVELS))) {
		tracepanicmsg("allocpage");
		return NULL;
	}

	/* lastpt is non-full, so it is assured that an invalid pte is found */
	pte = invalidentry(lastpt);

	if (!(f = palloc(PAGE_SIZE))) {
		tracepanicmsg("allocpage");
		return NULL;
	}

	*pte = PAGE_ENTRY_SET_PPN(*pte, (pageentry)f);
	*pte = PAGE_ENTRY_ADD_VALID(*pte);

	if (opts.u)
		*pte = PAGE_ENTRY_ADD_USER(*pte);

	if (opts.r)
		*pte = PAGE_ENTRY_ADD_R(*pte);

	if (opts.w)
		*pte = PAGE_ENTRY_ADD_W(*pte);

	if (opts.x)
		*pte = PAGE_ENTRY_ADD_X(*pte);

	return pte;
}

pageentry **
allocpagetable(void)
{
	pageentry **pt;

	if (!(pt = palloc(PAGE_TABLE_ENTRIES * sizeof(pageentry *)))) {
		tracepanicmsg("allocpagetable");
		return NULL;
	}

	return pt;
}

void
freepage(pageentry *pte, pageentry *pt[PAGE_TABLE_ENTRIES])
{
	pageentry f, **parents[PAGE_TABLE_LEVELS + 1];
	int i;

	parenttables(parents, pt, pte);

	/* If pte is not found in pt */
	if (!parents[0])
		return;

	f = PAGE_ENTRY_GET_PPN(*(pageentry *)pte);

	pfree((void *)f, PAGE_SIZE);

	*(pageentry *)pte = PAGE_ENTRY_REM_VALID(*(pageentry *)pte);

	/* Free parent page tables which became empty after pte removal */
	/* Skip root page table (i = 1) */
	for (i = 1; parents[i] && i < PAGE_TABLE_LEVELS; i++) {
		pageentry *ptrentry;

		/* If parents[i] is non-empty */
		if (validentry(parents[i]))
			continue;

		pfree(parents[i], PAGE_TABLE_ENTRIES * sizeof(pageentry *));

		/* Remove entry pointing to invalidated table */
		ptrentry = findpointerentry(parents[i - 1], parents[i]);

		*ptrentry = PAGE_ENTRY_REM_VALID(*ptrentry);
	}
}
