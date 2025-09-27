#include <pagetable.h>

#include <arch/page.h>
#include <arch/types.h>
#include <panic.h>
#include <pmem.h>

/* Returns 1 if the value in ptr is equal to e or 0 otherwise. */
static u8 equalentries(pageentry e, void *ptr);

/* To use with walkpagetree. Returns 1 if entry e is invalid or 0 otherwise. */
static u8 invalidentry(pageentry e, void *);

/* Returns the first non-full l-level page table it encounters in page tree pt,
   or NULL if pt is full. It creates one if needed. */
static pageentry **levelpagetable(pageentry *pt[PAGE_TABLE_ENTRIES], u8 l);

/* Makes tables a NULL terminated array containing, in order from root to close
   parent, the path of tables eventually pointing to page table entry pte.
   tables[0] is set to NULL if pte is not found. */
static void parenttables(pageentry **tables[PAGE_TABLE_LEVELS + 1],
                         pageentry *pt[PAGE_TABLE_ENTRIES],
                         pageentry *pte);

/* Returns the first valid entry in page table pt, or NULL if pt is empty. */
static pageentry *validentry(pageentry *pt[PAGE_TABLE_ENTRIES]);

u8
equalentries(pageentry e, void *ptr)
{
	return e == *(pageentry *)ptr;
}

u8
invalidentry(pageentry e, void *)
{
	return !PAGE_ENTRY_GET_VALID(e);
}

pageentry **
levelpagetable(pageentry *pt[PAGE_TABLE_ENTRIES], u8 l)
{
	u16 i, lvlidx[PAGE_TABLE_LEVELS] = { 0 };

	if (l < 1 && l > PAGE_TABLE_LEVELS) {
		setpanicmsg("Passed non-existent level.");
		tracepanicmsg("levelpagetable");
		return NULL;
	}

	for (i = 0; i < l - 1; i++) {
		/* We need to keep track of the last entry index of each level
		   in case we need to go backwards after walking through a
		   full table: lvlidx does that. */
		for (; lvlidx[i] < PAGE_TABLE_ENTRIES; lvlidx[i]++) {
			pageentry *pte;

			pte = (pageentry *)((uptr)pt
			                    + lvlidx[i] * sizeof(pageentry *));

			/* If invalid, create and walk in a new pt. */
			if (!PAGE_ENTRY_GET_VALID(*pte)) {
				pagetable(pt);

				*pte = PAGE_ENTRY_SET_PPN(*pte, (pageentry)pt);
				*pte = PAGE_ENTRY_ADD_VALID(*pte);
				*pte = PAGE_ENTRY_SET_WALKABLE(*pte);

				goto nextlevel;
			}

			if (PAGE_ENTRY_GET_WALKABLE(*pte)) {
				uptr addr = PAGE_ENTRY_GET_PPN(*pte);

				pt = (pageentry **)addr;

				goto nextlevel;
			}
		}

walkback:
		/* If we walked through the whole root page table, then it is
		   full. */
		if (lvlidx[0] >= PAGE_TABLE_ENTRIES - 1)
			return NULL;

		/* If none of the entries are walkable, there is no space in
		   pt: we need to go back one level and continue from where we
		   left. */
		i -= 2;
		continue;

nextlevel:
		if (i < l - 1)
			continue;

		/* Last iteration only. */

		/* If pt is full, walk back and continue. */
		if (walkpagetree(pt, invalidentry, NULL))
			goto walkback;
	}

	return pt;
}

void
parenttables(pageentry **tables[PAGE_TABLE_LEVELS + 1],
             pageentry *pt[PAGE_TABLE_ENTRIES],
             pageentry *pte)
{
	u16 i, lvlidx[PAGE_TABLE_LEVELS] = { 0 };

	/* Walk the whole tree saving each level to tables until pte is
	   found. */
	tables[0] = pt;
	for (i = 0; i < PAGE_TABLE_LEVELS; i++) {
		for (; lvlidx[i] < PAGE_TABLE_ENTRIES; lvlidx[i]++) {
			pageentry *e;

			e = (pageentry *)((uptr)tables[i]
			                  + lvlidx[i] * sizeof(pageentry *));

			if (!PAGE_ENTRY_GET_VALID(*e))
				continue;

			if ((uptr)e == (uptr)pte) {
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
		   not found. */
		if (lvlidx[0] >= PAGE_TABLE_ENTRIES - 1)
			break;

		/* If we walked through a table full of non-walkable entries,
		   walk back. */
		i -= 2;

nextlevel:
	}

	/* If no return, then pte was not found. */
	tables[0] = NULL;
}

pageentry *
validentry(pageentry *pt[PAGE_TABLE_ENTRIES])
{
	u16 i;

	for (i = 0; i < PAGE_TABLE_ENTRIES; i++) {
		pageentry *pte = (void *)((uptr)pt + i * sizeof(pageentry *));

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

	/* lastpt is non-full, so it is assured that an invalid pte is
	   found. */
	pte = walkpagetree(lastpt, invalidentry, NULL);

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

void
freepage(pageentry *pte, pageentry *pt[PAGE_TABLE_ENTRIES])
{
	pageentry f, **parents[PAGE_TABLE_LEVELS + 1];
	u16 i;

	parenttables(parents, pt, pte);

	/* If pte is not found in pt. */
	if (!parents[0])
		return;

	f = PAGE_ENTRY_GET_PPN(*(pageentry *)pte);

	pfree((void *)f, PAGE_SIZE);

	*(pageentry *)pte = PAGE_ENTRY_REM_VALID(*(pageentry *)pte);

	/* Free parent page tables which became empty after pte removal. */
	/* Skip root page table (i = 1). */
	for (i = 1; parents[i] && i < PAGE_TABLE_LEVELS; i++) {
		pageentry *ptrentry;

		/* If parents[i] is non-empty. */
		if (validentry(parents[i]))
			continue;

		pfree(parents[i], PAGE_TABLE_ENTRIES * sizeof(pageentry *));

		/* Remove entry pointing to invalidated table. */
		ptrentry = walkpagetree(parents[i - 1], equalentries,
		                        parents[i]);

		*ptrentry = PAGE_ENTRY_REM_VALID(*ptrentry);
	}
}

void
pagetable(pageentry *pt[PAGE_TABLE_ENTRIES])
{
	pmemset(pt, 0, PAGE_TABLE_ENTRIES * sizeof(pageentry *));
}

pageentry *
walkpagetree(pageentry *ptree[PAGE_TABLE_ENTRIES],
             u8 (*check)(pageentry, void *),
             void *extra)
{
	pageentry **ptable = ptree;
	u32 i;

rewalk:
	for (i = 0; i < PAGE_TABLE_ENTRIES; i++) {
		pageentry *pte = (pageentry *)((uptr)ptable
		                               + i * sizeof(pageentry *));

		if (check(*pte, extra))
			return pte;

		if (PAGE_ENTRY_GET_WALKABLE(*pte)) {
			ptable = (pageentry **)PAGE_ENTRY_GET_PPN(*pte);
			goto rewalk;
		}
	}

	return NULL;
}
