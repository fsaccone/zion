#include <pagetable.h>

#include <arch/page.h>
#include <arch/types.h>
#include <log.h>
#include <pmem.h>

/* The used scheme is Sv57 */

#define PAGE_TABLE_ENTRIES 512
#define PAGE_TABLE_LEVELS  5

/* Returns the first valid inner node in page tree pt which points to address
   ptr, or NULL if ptr is not found */
static u64 *findpointerentry(void *pt, void *ptr);

/* Returns the first invalid entry in page table pt, or NULL if pt is full */
static u64 *invalidentry(void *pt);

/* Returns the first non-full l-level page table it encounters, or NULL if pt
   is full. It creates one if needed */
static u64 *levelpagetable(void *pt, int l);

/* Makes tables a NULL terminated array containing, in order from root to close
   parent, the path of tables eventually pointing to page table entry pte.
   tables[0] is set to NULL if pte is not found */
static void parenttables(u64 *tables[PAGE_TABLE_LEVELS + 1], void *pt,
                         void *pte);

/* Returns the first valid entry in page table pt, or NULL if pt is empty */
static u64 *validentry(void *pt);

u64 *
findpointerentry(void *pt, void *ptr)
{
	void *curpt = pt;
	int i;

redowalk:
	for (i = 0; i < PAGE_TABLE_ENTRIES; i++) {
		u64 *pte = (void *)((un)curpt + i * sizeof(u64));
		u64 ppn;

		ppn = *pte;

		/* Set all option bits to 0 */
		ppn >>= 10;
		ppn <<= 10;

		/* Set upper reserved bits to 0 */
		ppn <<= 10;
		ppn >>= 10;

		if (ppn == (u64)ptr)
			return pte;

		/* If R, W and X are all 0, walk in the pt at PPN */
		if (!(*pte & 0b1110)) {
			u64 addr = *pte;

			/* Set all option bits to 0 */
			addr >>= 10;
			addr <<= 10;

			/* Set upper reserved bits to 0 */
			addr <<= 10;
			addr >>= 10;

			curpt = (u64 *)addr;

			goto redowalk;
		}
	}

	return NULL;
}

u64 *
invalidentry(void *pt)
{
	int i;

	for (i = 0; i < PAGE_TABLE_ENTRIES; i++) {
		u64 *pte = (void *)((un)pt + i * sizeof(u64));

		if (!(*pte & 1))
			return pte;
	}

	return NULL;
}

u64 *
levelpagetable(void *pt, int l)
{
	int i, lvlidx[PAGE_TABLE_LEVELS] = { 0 };
	u64 *lastpt;

	if (l < 1 && l > PAGE_TABLE_LEVELS) {
		setpanicmsg("Passed non-existent level.");
		return NULL;
	}

	lastpt = (u64 *)pt;
	for (i = 0; i < l - 1; i++) {
		/* We need to keep track of the last entry index of each level
		   in case we need to go backwards after walking through a
		   full table: lvlidx does that */
		for (; lvlidx[i] < PAGE_TABLE_ENTRIES; lvlidx[i]++) {
			u64 *pte = (void *)((un)lastpt
			                    + lvlidx[i] * sizeof(u64));

			/* If V is 0, create and walk in a new pt */
			if (!(*pte & 1)) {
				lastpt = createpagetable();

				/* Set PPN to point to new table (since lastpt
				   is aligned, upper reserved bits and option
				   bits are already 0) */
				*pte = (un)lastpt;

				/* Set V to 1 */
				*pte |= 1;

				goto nextlevel;
			}

			/* If R, W and X are all 0, walk in the pt at PPN */
			if (!(*pte & 0b1110)) {
				u64 addr = *pte;

				/* Set all option bits to 0 */
				addr >>= 10;
				addr <<= 10;

				/* Set upper reserved bits to 0 */
				addr <<= 10;
				addr >>= 10;

				lastpt = (u64 *)addr;

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
parenttables(u64 *tables[PAGE_TABLE_LEVELS + 1], void *pt, void *pte)
{
	int i, lvlidx[PAGE_TABLE_LEVELS] = { 0 };

	/* Walk the whole tree saving each level to tables until pte is
	   found */
	tables[0] = pt;
	for (i = 0; i < PAGE_TABLE_LEVELS; i++) {
		for (; lvlidx[i] < PAGE_TABLE_ENTRIES; lvlidx[i]++) {
			u64 *e = (void *)((un)tables[i]
			                  + lvlidx[i] * sizeof(u64));

			/* Skip invalid entries */
			if (!(*e & 0b1))
				continue;

			if ((un)e == (un)pte) {
				tables[i + 1] = NULL;
				return;
			}

			/* If R, W and X are all 0, walk in the pt at PPN */
			if (!(*e & 0b1110)) {
				u64 addr = *e;

				/* Set all option bits to 0 */
				addr >>= 10;
				addr <<= 10;

				/* Set upper reserved bits to 0 */
				addr <<= 10;
				addr >>= 10;

				tables[i + 1] = (u64 *)addr;

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

u64 *
validentry(void *pt)
{
	int i;

	for (i = 0; i < PAGE_TABLE_ENTRIES; i++) {
		u64 *pte = (void *)((un)pt + i * sizeof(u64));

		if (*pte & 1)
			return pte;
	}

	return NULL;
}

void *
allocpage(void *pt, struct pageoptions opts)
{
	u64 *lastpt, *pte;
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
	*pte |= (un)f;

	/* V bit */
	*pte |= 1;

	/* U bit */
	*pte |= opts.u << 4;

	/* R bit */
	*pte |= opts.r << 1;

	/* W bit */
	*pte |= opts.w << 2;

	/* X bit */
	*pte |= opts.x << 3;

	return pte;
}

void *
createpagetable(void)
{
	void *pt;
	/* Since palloc allocates frames aligned with PAGE_SIZE=4096, it is
	   assured that the page table is also aligned to 4096. Also, palloc
	   fills all allocated frames with zeros, and that is what we want. */
	if (!(pt = palloc(PAGE_TABLE_ENTRIES * sizeof(un))))
		panic("palloc");

	return pt;
}

void
freepage(void *pte, void *pt)
{
	u64 f, *parents[PAGE_TABLE_LEVELS + 1];
	int i;

	parenttables(parents, pt, pte);

	/* If pte is not found in pt */
	if (!parents[0])
		return;

	f = *(u64 *)pte;

	/* Set all option bits to 0 */
	f >>= 10;
	f <<= 10;

	/* Set upper reserved bits to 0 */
	f <<= 10;
	f >>= 10;

	pfree((void *)f, PAGE_SIZE);

	*(u64 *)pte = 0;

	/* Free parent page tables which became empty after pte removal */
	/* Skip root page table (i = 1) */
	for (i = 1; parents[i] && i < PAGE_TABLE_LEVELS; i++) {
		void *ptrentry;

		/* If parents[i] is non-empty */
		if (validentry(parents[i]))
			continue;

		pfree(parents[i], PAGE_TABLE_ENTRIES * sizeof(un));

		/* Remove entry pointing to invalidated table */
		ptrentry = findpointerentry(parents[i - 1], parents[i]);

		*(u64 *)ptrentry = 0;
	}
}
