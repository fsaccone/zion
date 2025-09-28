#include <vmem.h>

#include <arch/page.h>
#include <arch/types.h>
#include <pmem.h>

struct walklevel {
	u32 i;
	pageentry *ptable;
};

/* Check function of walkpagetree. Returns 1 if entry e is an invalid entry, or
   0 otherwise. */
static u8 invalidentry(struct pte e, void *);

/* Check function of walkpagetree. Returns 1 if entry e is an invalid entry
   preceding *(u32 *)n - 1 invalid entries in its table, or 0 otherwise. */
static u8 startsninvalidentries(struct pte e, void *n);

u8
invalidentry(struct pte e, void *)
{
	return !PAGE_ENTRY_GET_VALID(e.ptable[e.i]);
}

u8
startsninvalidentries(struct pte e, void *n)
{
	u32 i, ne = *(u32 *)n;

	/* If not enough space in e.ptable. */
	if (e.i + ne > PAGE_TABLE_ENTRIES)
		return 0;

	for (i = e.i; i < e.i + ne; i++) {
		if (PAGE_ENTRY_GET_VALID(e.ptable[i]))
			return 0;
	}

	return 1;
}

u8
walkpagetree(struct pte *o,
             pageentry ptree[PAGE_TABLE_ENTRIES],
             u8 minlvl, u8 maxlvl,
             u8 (*check)(struct pte, void *),
             void *extra)
{
	u32 l;
	struct walklevel levels[PAGE_TABLE_LEVELS] = { 0 };

	/* Limit maxlvl to PAGE_TABLE_LEVELS - 1. */
	if (maxlvl > PAGE_TABLE_LEVELS - 1)
		maxlvl = PAGE_TABLE_LEVELS - 1;

	if (minlvl > maxlvl)
		return -1;

	levels[0].ptable = ptree;

	for (l = 0; l <= maxlvl; l++) {
		for (; levels[l].i < PAGE_TABLE_ENTRIES; levels[l].i++) {
			struct pte e;
			pageentry ev;

			e.ptable = levels[l].ptable;
			e.i = levels[l].i;

			ev  = e.ptable[e.i];

			/* Only call check if we are at least in level
			   minlvl. */
			if (l >= minlvl && check(e, extra)) {
				*o = e;
				return 0;
			}

			/* If maxlvl was still not reached, walk in walkable
			   entries. */
			if (l < maxlvl && PAGE_ENTRY_GET_WALKABLE(ev)) {
				uptr paddr;

				paddr = PAGE_ENTRY_GET_PADDR(ev);

				levels[l + 1].ptable = (pageentry *)paddr;

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

	return -1;
}
