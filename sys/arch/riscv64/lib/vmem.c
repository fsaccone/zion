#include <vmem.h>

#include <arch.h>
#include <log.h>
#include <pmem.h>

/* The used scheme is Sv57 */

#define PAGE_TABLE_ENTRIES 512
#define PAGE_TABLE_LEVELS  5

/* Returns the first invalid entry in page table pt, or NULL if pt is full */
static uint64 *invalidentry(void *pt);

/* Returns the first valid l-level page table it encounters, or NULL if pt is
   full. It creates one if needed */
static uint64 *levelpagetable(void *pt, int l);

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

		/* If we walked through the whole root page table, then it is
		   full */
		if (lvlidx[0] == PAGE_TABLE_ENTRIES - 1)
			return NULL;

		/* If none of the entries are walkable, there is no space in
		   lastpt: we need to go back one level and continue from
		   where we left */
		i -= 2;
nextlevel:
	}

	return lastpt;
}

void *
createpagetable(void)
{
	/* Since palloc allocates frames aligned with PAGE_SIZE=4096, it is
	   assured that the page table is also aligned to 4096. Also, palloc
	   fills all allocated frames with zeros, and that is what we want. */
	return palloc(PAGE_TABLE_ENTRIES * sizeof(uintn));
}
