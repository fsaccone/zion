#include <vmem.h>

#include <arch/page.h>
#include <arch/types.h>
#include <panic.h>
#include <pmem.h>

struct getninvalidstate {
	uptr n;
	u32 lvlidxs[PAGE_TABLE_LEVELS];
	struct ptenode *cur;
	struct ptenode *res;
};

struct ptenode {
	struct pte e;
	struct ptenode *n;
};

struct walklevel {
	u32 i;
	pageentry *ptable;
};

/* Check function of walkpagetree. Parameter state must be of type struct
   getninvalidstate *. Returns 0 and appends last-level entry e to state->cur
   if it is invalid or sets state->cur to NULL if it is valid. If the length of
   state->cur reaches state->n, it is copied to state->res and 1 is returned.
   The state->lvlidxs array holds the index of each level leading to the first
   element of the state->cur linked list when 1 is returned. Returns -1 in case
   of error. To append entries to the linked lists, it allocates nodes using
   pmem. This function is based on the assumption that walkpagetree walks the
   tree entries in order. */
static s8 getninvalid(struct pte e, void *state);

s8
getninvalid(struct pte e, void *state)
{
	struct getninvalidstate *s = (struct getninvalidstate *)state;
	struct ptenode *newpn, *tail, *pn;
	uptr c;

	/* Always save level index of non-last-level entries and save level
	   index of a last-level entry only if it is the first of the linked
	   list. */
	if (e.l < PAGE_TABLE_LEVELS - 1 || !s->cur)
		s->lvlidxs[e.l] = e.i;

	/* Only check last level entries. */
	if (e.l < PAGE_TABLE_LEVELS - 1)
		return 0;

	if (PAGE_ENTRY_GET_VALID(e.ptable[e.i])) {
		struct ptenode *pnnext;

		/* Free s->cur. */
		for (pn = s->cur; pn; pn = pnnext) {
			pnnext = pn->n;
			if (pfree(pn, sizeof(struct ptenode))) {
				tracepanicmsg("getninvalid");
				return -1;
			}
		}

		s->cur = NULL;

		return 0;
	}

	/* Entry e is invalid. */

	if (!(newpn = palloc(sizeof(struct ptenode), 0))) {
		tracepanicmsg("getninvalid");
		return -1;
	}

	pmemcpy(&newpn->e, &e, sizeof(struct pte));

	if (!s->cur) {
		s->cur = newpn;
	} else {
		/* Get tail. */
		for (tail = s->cur; tail->n; tail = tail->n);

		tail->n = newpn;
	}

	for (c = 1, pn = s->cur; pn; pn = pn->n, c++) {
		if (c < s->n)
			continue;

		s->res = s->cur;
		return 1;
	}

	return 0;
}

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
walkpagetree(struct pte *o, pageentry ptree[PAGE_TABLE_ENTRIES], u8 maxlvl,
             s8 (*check)(struct pte, void *), void *extra)
{
	u32 l;
	struct walklevel levels[PAGE_TABLE_LEVELS] = { 0 };

	/* Limit maxlvl to PAGE_TABLE_LEVELS - 1. */
	if (maxlvl > PAGE_TABLE_LEVELS - 1)
		maxlvl = PAGE_TABLE_LEVELS - 1;

	levels[0].ptable = ptree;

	for (l = 0; l <= maxlvl; l++) {
		for (; levels[l].i < PAGE_TABLE_ENTRIES; levels[l].i++) {
			struct pte e;
			pageentry ev;
			s8 ret;

			e.ptable = levels[l].ptable;
			e.l      = l;
			e.i      = levels[l].i;

			ev  = e.ptable[e.i];

			if ((ret = check(e, extra))) {
				if (ret == -1) {
					tracepanicmsg("walkpagetree");
					return -1;
				}

				if (o)
					*o = e;

				return 0;
			}

			/* If maxlvl was still not reached, walk in walkable
			   entries. */
			if (l < maxlvl && PAGE_ENTRY_GET_WALKABLE(ev)) {
				uptr paddr;

				paddr = PAGE_ENTRY_GET_PADDR(ev);

				levels[l + 1].ptable = (pageentry *)paddr;
				levels[l + 1].i = 0;

				/* Set current entry as checked. */
				levels[l].i++;

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

	return 1;
}
