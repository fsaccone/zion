#include <pagetable.h>

#include <arch/page.h>
#include <arch/types.h>
#include <pmem.h>

struct walklevel {
	u32 i;
	pageentry **ptable;
};

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
	u32 l;
	struct walklevel levels[PAGE_TABLE_LEVELS] = { 0 };

	/* Limit maxlvl to PAGE_TABLE_LEVELS - 1. */
	if (maxlvl > PAGE_TABLE_LEVELS - 1)
		maxlvl = PAGE_TABLE_LEVELS - 1;

	if (minlvl > maxlvl)
		return NULL;

	levels[0].ptable = ptree;

	for (l = 0; l <= maxlvl; l++) {
		for (; levels[l].i < PAGE_TABLE_ENTRIES; levels[l].i++) {
			pageentry *pte;

			pte = (pageentry *)((uptr)levels[l].ptable
			                    + levels[l].i
			                      * sizeof(pageentry *));

			/* Only call check if we are at least in level
			   minlvl. */
			if (l >= minlvl && check (*pte, extra))
				return pte;

			/* If maxlvl was still not reached, walk in walkable
			   entries. */
			if (l < maxlvl && PAGE_ENTRY_GET_WALKABLE(*pte)) {
				levels[l + 1].ptable
				             = (pageentry **)
				               PAGE_ENTRY_GET_PPN(*pte);
				goto nextlevel;
			}
		}

		if (l == maxlvl) {
			u32 i;

			/* If we already walked through all the parent root
			   tables, that means we checked all the entries and
			   should break to return; else, we should just walk
			   back as normal. */
			for (i = 0; i < maxlvl; i++) {
				if (levels[i].i < PAGE_TABLE_ENTRIES - 1)
					goto walkback;
			}

			break;
		}

walkback:
		l -= 2;
nextlevel:
	}

	return NULL;
}
