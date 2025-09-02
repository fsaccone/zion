#include <vmem.h>

#include <arch.h>
#include <log.h>
#include <pmem.h>

/* The used scheme is Sv57 */

#define PAGE_TABLE_ENTRIES 512
#define PAGE_TABLE_LEVELS  5

/* Returns the first invalid entry in page table pt, or NULL if pt is full */
static uint64 *invalidentry(void *pt);

/* Returns the first non-full l-level page table it encounters, or NULL if pt
   is full. It creates one if needed */
static uint64 *levelpagetable(void *pt, int l);

/* Returns a NULL terminated array of size PAGE_TABLE_LEVELS containing, in
   order from root to close parent, the path of tables eventually pointing to
   page table entry pte. Returns NULL if pte is not found */
static uint64 **parenttables(void *pt, void *pte);

static uint64 *
invalidentry(void *pt)
{
	int i;

	for (i = 0; i < PAGE_TABLE_ENTRIES; i++) {
		uint64 *pte = (void *)((uintn)pt + i * sizeof(uint64));

		if (!(*pte & 1))
			return pte;
	}

	return NULL;
}

static uint64 *
levelpagetable(void *pt, int l)
{
	int i, lvlidx[PAGE_TABLE_LEVELS] = { 0 };
	uint64 *lastpt;

	if (l < 1 && l > PAGE_TABLE_LEVELS)
		panic("lastlevelpt: Passed non-existent level");

	lastpt = (uint64 *)pt;
	for (i = 0; i < l - 1; i++) {
		/* We need to keep track of the last entry index of each level
		   in case we need to go backwards after walking through a
		   full table: lvlidx does that */
		for (; lvlidx[i] < PAGE_TABLE_ENTRIES; lvlidx[i]++) {
			uint64 *pte = (void *)((uintn)lastpt
			                       + lvlidx[i] * sizeof(uint64));

			/* If V is 0, create and walk in a new pt */
			if (!(*pte & 1)) {
				lastpt = createpagetable();

				/* Set PPN to point to new table (since lastpt
				   is aligned, upper reserved bits and option
				   bits are already 0) */
				*pte = (uintn)lastpt;

				/* Set V to 1 */
				*pte |= 1;

				goto nextlevel;
			}

			/* If R, W and X are all 0, walk in the pt at PPN */
			if (!(*pte & 0b1110)) {
				uint64 addr = *pte;

				/* Set all option bits to 0 */
				addr >>= 10;
				addr <<= 10;

				/* Set upper reserved bits to 0 */
				addr <<= 10;
				addr >>= 10;

				lastpt = (uint64 *)addr;

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

static uint64 **
parenttables(void *pt, void *pte)
{
	int i, lvlidx[PAGE_TABLE_LEVELS] = { 0 };
	uint64 **tables = palloc((PAGE_TABLE_LEVELS + 1) * sizeof(uint64 *));

	/* Walk the whole tree saving each level to tables until pte is
	   found */
	tables[0] = pt;
	for (i = 0; i < PAGE_TABLE_LEVELS; i++) {
		for (; lvlidx[i] < PAGE_TABLE_ENTRIES; lvlidx[i]++) {
			uint64 *e = (void *)((uintn)tables[i]
			                     + lvlidx[i] * sizeof(uint64));

			/* Skip invalid entries */
			if (!(*e & 0b1))
				continue;

			if ((uintn)e == (uintn)pte) {
				tables[i + 1] = NULL;
				goto done;
			}

			/* If R, W and X are all 0, walk in the pt at PPN */
			if (!(*e & 0b1110)) {
				uint64 addr = *e;

				/* Set all option bits to 0 */
				addr >>= 10;
				addr <<= 10;

				/* Set upper reserved bits to 0 */
				addr <<= 10;
				addr >>= 10;

				tables[i + 1] = (uint64 *)addr;

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

	/* If not jump to done was done, then pte was not found */
	pfree(tables, PAGE_TABLE_LEVELS * sizeof(uint64 *));
	return NULL;

done:
	return tables;
}

void *
createpagetable(void)
{
	/* Since palloc allocates frames aligned with PAGE_SIZE=4096, it is
	   assured that the page table is also aligned to 4096. Also, palloc
	   fills all allocated frames with zeros, and that is what we want. */
	return palloc(PAGE_TABLE_ENTRIES * sizeof(uintn));
}

void *
valloc(void *pt, struct pageoptions opts)
{
	uint64 *lastpt, *pte;
	void *f;

	if (!opts.r && !opts.w && !opts.x)
		panic("addpage: No permissions passed.");

	if (opts.x && !opts.r)
		panic("addpage: Execute permission passed without read one.");

	if (!(lastpt = levelpagetable(pt, PAGE_TABLE_LEVELS)))
		return NULL;

	/* lastp is non-full, so it is assured that pte is valid */
	pte = invalidentry(lastpt);

	/* PPN bits */
	f = palloc(PAGE_SIZE);
	/* Since f is already aligned to PAGE_SIZE, there is no need to zero
	   the option bits */
	*pte |= (uintn)f;

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

void
vfree(void *pte)
{
	uint64 f = *(uint64 *)pte;

	/* Set all option bits to 0 */
	f >>= 10;
	f <<= 10;

	/* Set upper reserved bits to 0 */
	f <<= 10;
	f >>= 10;

	pfree((void *)f, PAGE_SIZE);

	*(uint64 *)pte = 0;
}
