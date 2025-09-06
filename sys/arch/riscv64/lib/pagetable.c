#include <pagetable.h>

#include <arch/page.h>
#include <arch/types.h>
#include <log.h>
#include <pmem.h>

/* Returns the first valid inner node in page tree pt which points to address
   ptr, or NULL if ptr is not found */
static pageentry *findpointerentry(void *pt, void *ptr);

/* Returns the first invalid entry in page table pt, or NULL if pt is full */
static pageentry *invalidentry(void *pt);

/* Returns the first non-full l-level page table it encounters, or NULL if pt
   is full. It creates one if needed */
static pageentry *levelpagetable(void *pt, int l);

/* Makes tables a NULL terminated array containing, in order from root to close
   parent, the path of tables eventually pointing to page table entry pte.
   tables[0] is set to NULL if pte is not found */
static void parenttables(pageentry *tables[PAGE_TABLE_LEVELS + 1], void *pt,
                         void *pte);

/* Returns the first valid entry in page table pt, or NULL if pt is empty */
static pageentry *validentry(void *pt);

pageentry *
findpointerentry(void *pt, void *ptr)
{
	void *curpt = pt;
	int i;

redowalk:
	for (i = 0; i < PAGE_TABLE_ENTRIES; i++) {
		pageentry *pte = (void *)((un)curpt + i * sizeof(pageentry *));
		pageentry ppn;

		ppn = PAGE_ENTRY_GET_PPN(*pte);

		if (ppn == (pageentry)ptr)
			return pte;

		if (PAGE_ENTRY_GET_WALKABLE(*pte)) {
			curpt = (pageentry *)ppn;

			goto redowalk;
		}
	}

	return NULL;
}

pageentry *
invalidentry(void *pt)
{
	int i;

	for (i = 0; i < PAGE_TABLE_ENTRIES; i++) {
		pageentry *pte = (void *)((un)pt + i * sizeof(pageentry *));

		if (!PAGE_ENTRY_GET_VALID(*pte))
			return pte;
	}

	return NULL;
}

pageentry *
levelpagetable(void *pt, int l)
{
	int i, lvlidx[PAGE_TABLE_LEVELS] = { 0 };
	pageentry *lastpt;

	if (l < 1 && l > PAGE_TABLE_LEVELS) {
		setpanicmsg("Passed non-existent level.");
		return NULL;
	}

	lastpt = (pageentry *)pt;
	for (i = 0; i < l - 1; i++) {
		/* We need to keep track of the last entry index of each level
		   in case we need to go backwards after walking through a
		   full table: lvlidx does that */
		for (; lvlidx[i] < PAGE_TABLE_ENTRIES; lvlidx[i]++) {
			pageentry *pte = (void *)((un)lastpt
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
				pageentry addr = PAGE_ENTRY_GET_PPN(*pte);

				lastpt = (pageentry *)addr;

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
parenttables(pageentry *tables[PAGE_TABLE_LEVELS + 1], void *pt, void *pte)
{
	int i, lvlidx[PAGE_TABLE_LEVELS] = { 0 };

	/* Walk the whole tree saving each level to tables until pte is
	   found */
	tables[0] = pt;
	for (i = 0; i < PAGE_TABLE_LEVELS; i++) {
		for (; lvlidx[i] < PAGE_TABLE_ENTRIES; lvlidx[i]++) {
			pageentry *e = (void *)((un)tables[i]
			                  + lvlidx[i] * sizeof(pageentry *));

			/* Skip invalid entries */
			if (!PAGE_ENTRY_GET_VALID(*e))
				continue;

			if ((un)e == (un)pte) {
				tables[i + 1] = NULL;
				return;
			}

			if (PAGE_ENTRY_GET_WALKABLE(*e)) {
				pageentry addr = PAGE_ENTRY_GET_PPN(*e);

				tables[i + 1] = (pageentry *)addr;

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
validentry(void *pt)
{
	int i;

	for (i = 0; i < PAGE_TABLE_ENTRIES; i++) {
		pageentry *pte = (void *)((un)pt + i * sizeof(pageentry *));

		if (PAGE_ENTRY_GET_VALID(*pte))
			return pte;
	}

	return NULL;
}

void *
allocpage(void *pt, struct pageoptions opts)
{
	pageentry *lastpt, *pte;
	void *f;

	if (!opts.r && !opts.w && !opts.x) {
		setpanicmsg("No permissions passed.");
		return NULL;
	}

	if (opts.x && !opts.r) {
		setpanicmsg("Execute permission passed without read one.");
		return NULL;
	}

	if (!(lastpt = levelpagetable(pt, PAGE_TABLE_LEVELS)))
		return NULL;

	/* lastp is non-full, so it is assured that pte is valid */
	pte = invalidentry(lastpt);

	/* PPN bits */
	if (!(f = palloc(PAGE_SIZE)))
		panic("palloc");
	/* Since f is already aligned to PAGE_SIZE, there is no need to zero
	   the option bits */
	*pte = PAGE_ENTRY_SET_PPN(*pte, (pageentry)f);

	/* V bit */
	*pte = PAGE_ENTRY_ADD_VALID(*pte);

	/* U bit */
	if (opts.u)
		*pte = PAGE_ENTRY_ADD_USER(*pte);

	/* R bit */
	if (opts.r)
		*pte = PAGE_ENTRY_ADD_R(*pte);

	/* W bit */
	if (opts.w)
		*pte = PAGE_ENTRY_ADD_W(*pte);

	/* X bit */
	if (opts.x)
		*pte = PAGE_ENTRY_ADD_X(*pte);

	return pte;
}

void *
allocpagetable(void)
{
	void *pt;
	/* Since palloc allocates frames aligned with PAGE_SIZE=4096, it is
	   assured that the page table is also aligned to 4096. Also, palloc
	   fills all allocated frames with zeros, and that is what we want. */
	if (!(pt = palloc(PAGE_TABLE_ENTRIES * sizeof(pageentry *))))
		panic("palloc");

	return pt;
}

void
freepage(void *pte, void *pt)
{
	pageentry f, *parents[PAGE_TABLE_LEVELS + 1];
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
		void *ptrentry;

		/* If parents[i] is non-empty */
		if (validentry(parents[i]))
			continue;

		pfree(parents[i], PAGE_TABLE_ENTRIES * sizeof(pageentry *));

		/* Remove entry pointing to invalidated table */
		ptrentry = findpointerentry(parents[i - 1], parents[i]);

		*(pageentry *)ptrentry
		              = PAGE_ENTRY_REM_VALID(*(pageentry *)ptrentry);
	}
}
