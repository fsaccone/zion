#include <pagetable.h>

#include <arch/page.h>
#include <arch/types.h>
#include <pmem.h>

/* To use with walkpagetree. Returns 1 if the value in ptr is equal to e or 0
   otherwise. */
static s8 equalentries(pageentry e, void *ptr);

/* To use with walkpagetree. Returns 1 if entry e is invalid or 0 otherwise. */
static s8 invalidentry(pageentry e, void *);

/* To use with walkpagetree. Returns 1 if entry e is valid or 0 otherwise. */
static s8 validentry(pageentry e, void *);

s8
equalentries(pageentry e, void *ptr)
{
	return e == *(pageentry *)ptr;
}

s8
invalidentry(pageentry e, void *)
{
	return !PAGE_ENTRY_GET_VALID(e);
}


s8
validentry(pageentry e, void *)
{
	return PAGE_ENTRY_GET_VALID(e);
}

void
pagetable(pageentry *ptable[PAGE_TABLE_ENTRIES])
{
	pmemset(ptable, 0, PAGE_TABLE_ENTRIES * sizeof(pageentry *));
}

pageentry *
walkpagetree(pageentry *ptree[PAGE_TABLE_ENTRIES],
             u8 minlvl, u8 maxlvl,
             s8 (*check)(pageentry, void *),
             void *extra)
{
	pageentry **ptable = ptree;
	u32 l, lvlidxs[PAGE_TABLE_LEVELS] = { 0 };

	/* Limit maxlvl to PAGE_TABLE_LEVELS - 1. */
	if (maxlvl > PAGE_TABLE_LEVELS - 1)
		maxlvl = PAGE_TABLE_LEVELS - 1;

	if (minlvl > maxlvl)
		return NULL;

	for (l = 0; l <= maxlvl; l++) {
		for (; lvlidxs[l] < PAGE_TABLE_ENTRIES; lvlidxs[l]++) {
			pageentry *pte;

			pte = (pageentry *)((uptr)ptable
			                    + lvlidxs[l]
			                      * sizeof(pageentry *));

			/* Only call check if we are at least in level
			   minlvl. */
			if (l >= minlvl) {
				switch (check(*pte, extra)) {
				case -1:
					goto walkback;
				case 1:
					return pte;
				default:
				}
			}

			if (PAGE_ENTRY_GET_WALKABLE(*pte)) {
				ptable = (pageentry **)
				         PAGE_ENTRY_GET_PPN(*pte);
				goto nextlevel;
			}
		}

		/* If we got here, the  walked level is full and it contains no
		   walkable entries. */
		break;

walkback:
		l -= 2;
nextlevel:
	}

	return NULL;
}
