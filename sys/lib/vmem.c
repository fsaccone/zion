#include <vmem.h>

#include <arch/page.h>
#include <arch/types.h>
#include <math.h>
#include <panic.h>
#include <pmem.h>

struct getninvalidstate {
	/* Inputs. */
	uptr n;

	/* Outputs. */
	uptr lvlidxs[PAGE_TABLE_LEVELS];
	struct ptenode *entries;

	/* Private. */
	uptr c;
	struct ptenode *entriestail;
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
   getninvalidstate *.

   Returns 0 and appends last-level entry e to
   state->entries if it is invalid or frees state->entries if it is valid.

   If the length of state->entries, saved in state->c, reaches state->n, 1 is
   returned.

   The state->lvlidxs array holds the index of each level leading to the first
   element of the state->entries linked list when 1 is returned.

   Returns -1 in case of error.

   To append entries to the state->entries linked list, it allocates nodes
   using pmem. This function is based on the assumption that walkpagetree walks
   the tree entries in order. */
static s8 getninvalid(struct pte e, void *state);

/* Return the single integer representing the entry pointed by level indexes
   lvlidxs. */
static uptr idxfromlvlidxs(uptr lvlidxs[PAGE_TABLE_LEVELS]);

/* Check function of walkpagetree. Returns 1 if entry e is valid or 0
   otherwise. */
static s8 validentry(struct pte e, void *);

s8
getninvalid(struct pte e, void *state)
{
	struct getninvalidstate *s = (struct getninvalidstate *)state;
	struct ptenode *newpn, *pn;

	/* Always save level index of non-last-level entries and save level
	   index of a last-level entry only if it is the first of the linked
	   list. */
	if (!s->entries)
		s->lvlidxs[e.l] = e.i;

	/* Only check last level entries. */
	if (e.l < PAGE_TABLE_LEVELS - 1)
		return 0;

	if (PAGE_ENTRY_GET_VALID(e.ptable[e.i])) {
		struct ptenode *pnnext;

		/* Free s->entries. */
		for (pn = s->entries; pn; pn = pnnext) {
			pnnext = pn->n;
			if (pfree(pn, sizeof(struct ptenode))) {
				tracepanicmsg("getninvalid");
				return -1;
			}
		}

		s->entries = NULL;

		/* Reset count. */
		s->c = 0;

		return 0;
	}

	/* Entry e is invalid. */

	if (!(newpn = palloc(sizeof(struct ptenode), 0))) {
		tracepanicmsg("getninvalid");
		return -1;
	}

	pmemcpy(&newpn->e, &e, sizeof(struct pte));

	if (!s->entries) {
		s->entries = newpn;
	} else {
		s->entriestail->n = newpn;
		s->entriestail = newpn;
	}

	if (++s->c >= s->n)
		return 1;

	return 0;
}

uptr
idxfromlvlidxs(uptr lvlidxs[PAGE_TABLE_LEVELS])
{
	uptr i, idx = 0;

	for (i = 0; i < PAGE_TABLE_LEVELS; i++) {
		uptr l = PAGE_TABLE_LEVELS - 1 - i;

		idx += l * PAGE_TABLE_ENTRIES * lvlidxs[l];
	}

	return idx;
}

s8
validentry(struct pte e, void *)
{
	return PAGE_ENTRY_GET_VALID(e.ptable[e.i]);
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
valloc(pageentry ptree[PAGE_TABLE_ENTRIES], uptr vaddr,
       struct pageoptions opts)
{
	uptr l, lvlidxs[PAGE_TABLE_LEVELS] = PAGE_LVLIDXS_FROM_VADDR(vaddr);
	pageentry *lastpt, *e;
	void *f;

	/* Get last-level page table pointing to e starting from ptree. */
	lastpt = ptree;
	for (l = 0; l < PAGE_TABLE_LEVELS; l++) {
		pageentry *lastpte = &lastpt[lvlidxs[l]];

		/* Allocate invalid intermediate page tables. */
		if (!PAGE_ENTRY_GET_VALID(*lastpte)) {
			pageentry *newpt;

			if (!(newpt = allocpagetable())) {
				tracepanicmsg("valloc");
				return -1;
			}

			*lastpte = PAGE_ENTRY_ADD_VALID(*lastpte);
			*lastpte = PAGE_ENTRY_SET_PADDR(*lastpte, (uptr)newpt);
		}

		lastpt = (pageentry *)PAGE_ENTRY_GET_PADDR(*lastpte);
	}

	e = &lastpt[lvlidxs[PAGE_TABLE_LEVELS - 1]];

	if (!(f = palloc(PAGE_SIZE, 0))) {
		tracepanicmsg("valloc");
		return -1;
	}

	*e = PAGE_ENTRY_SET_PADDR(*e, (uptr)f);

	*e = PAGE_ENTRY_ADD_VALID(*e);

	if (opts.r)
		*e = PAGE_ENTRY_ADD_R(*e);

	if (opts.w)
		*e = PAGE_ENTRY_ADD_W(*e);

	if (opts.x)
		*e = PAGE_ENTRY_ADD_X(*e);

	if (opts.u)
		*e = PAGE_ENTRY_ADD_USER(*e);

	return 0;
}

s8
vfree(pageentry ptree[PAGE_TABLE_ENTRIES], uptr vaddr)
{
	uptr l, i, lvlidxs[PAGE_TABLE_LEVELS] = PAGE_LVLIDXS_FROM_VADDR(vaddr);
	pageentry *lastpt, *e;
	void *f;

	/* Get last-level page table pointing to e starting from ptree. */
	lastpt = ptree;
	for (l = 0; l < PAGE_TABLE_LEVELS; l++) {
		pageentry *lastpte = &lastpt[lvlidxs[l]];

		if (!PAGE_ENTRY_GET_VALID(*lastpte)) {
			setpanicmsg("Invalid page.");
			tracepanicmsg("vfree");
			return -1;
		}

		lastpt = (pageentry *)PAGE_ENTRY_GET_PADDR(*lastpte);
	}

	e = &lastpt[lvlidxs[PAGE_TABLE_LEVELS - 1]];

	if (!PAGE_ENTRY_GET_VALID(*e)) {
		setpanicmsg("Invalid page.");
		tracepanicmsg("vfree");
		return -1;
	}

	f = (void *)PAGE_ENTRY_GET_PADDR(*e);

	if (pfree(f, PAGE_SIZE)) {
		tracepanicmsg("vfree");
		return -1;
	}

	*e = PAGE_ENTRY_REM_VALID(*e);

	/* Check if any page table is empty (i.e. full of invalid entries): if
	   it is, free it. */
	for (i = 0; i < PAGE_TABLE_LEVELS - 1; i++) {
		pageentry *pt, *etopt;
		uptr j, ei;
		u8 isempty;

		/* Find current page table pt and the entry pointing to it,
		   etopt, which is needed when pt has to be freed. */
		pt = ptree;
		for (j = 0; j < PAGE_TABLE_LEVELS - i - 1; j++) {
			/* Set etopt on the previous pt, which is the parent of
			   the pt which is going to be set next. */
			etopt = &pt[lvlidxs[j]];

			pt = (pageentry *)PAGE_ENTRY_GET_PADDR(pt[lvlidxs[j]]);
		}

		/* Set isempty. */
		isempty = 1;
		for (ei = 0; ei < PAGE_TABLE_ENTRIES && isempty; ei++) {
			if (PAGE_ENTRY_GET_VALID(pt[ei]))
				isempty = 0;
		}

		if (isempty) {
			/* Free and invalidate page table. */
			if (pfree(pt,
			          sizeof(pageentry) * PAGE_TABLE_ENTRIES)) {
				tracepanicmsg("vfree");
				return -1;
			}

			*etopt = PAGE_ENTRY_REM_VALID(*etopt);
		}
	}

	return 0;
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
