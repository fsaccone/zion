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
   getninvalidstate *. Returns 0 and appends last-level entry e to
   state->entries if it is invalid or frees state->entries if it is valid. If
   the length of state->entries, saved in state->c, reaches state->n, 1 is
   returned. The state->lvlidxs array holds the index of each level leading to
   the first element of the state->entries linked list when 1 is returned.
   Returns -1 in case of error. To append entries to the linked list, it
   allocates nodes using pmem. This function is based on the assumption that
   walkpagetree walks the tree entries in order. */
static s8 getninvalid(struct pte e, void *state);

/* Check function of walkpagetree. Returns 1 if entry e is valid or 0
   otherwise. */
static s8 validentry(struct pte e, void *);

s8
getninvalid(struct pte e, void *state)
{
	struct getninvalidstate *s = (struct getninvalidstate *)state;
	struct ptenode *newpn, *tail, *pn;

	/* Always save level index of non-last-level entries and save level
	   index of a last-level entry only if it is the first of the linked
	   list. */
	if (e.l < PAGE_TABLE_LEVELS - 1 || !s->entries)
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
		/* Get tail. */
		for (tail = s->entries; tail->n; tail = tail->n);

		tail->n = newpn;
	}

	if (++s->c >= s->n)
		return 1;

	return 0;
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
valloc(uptr *o, pageentry ptree[PAGE_TABLE_ENTRIES], struct pageoptions opts,
       uptr s)
{
	struct getninvalidstate state = { 0 };
	struct ptenode *e;

	state.n = CEIL(s, PAGE_SIZE) / PAGE_SIZE;

	switch (walkpagetree(NULL, ptree, PAGE_TABLE_LEVELS - 1, getninvalid,
	                     &state)) {
	case -1:
		tracepanicmsg("valloc");
		return -1;
	case 1:
		return 1;
	default:
	}

	for (e = state.entries; e; e = e->n) {
		void *f;
		pageentry *eptr = &e->e.ptable[e->e.i];

		if (!(f = palloc(PAGE_SIZE, 0))) {
			tracepanicmsg("valloc");
			return -1;
		}

		*eptr = PAGE_ENTRY_SET_PADDR(*eptr, (uptr)f);

		*eptr = PAGE_ENTRY_ADD_VALID(*eptr);

		if (opts.r)
			*eptr = PAGE_ENTRY_ADD_R(*eptr);

		if (opts.w)
			*eptr = PAGE_ENTRY_ADD_W(*eptr);

		if (opts.x)
			*eptr = PAGE_ENTRY_ADD_X(*eptr);

		if (opts.u)
			*eptr = PAGE_ENTRY_ADD_USER(*eptr);
	}

	if (o)
		*o = PAGE_VADDR_FROM_LVLIDXS(state.lvlidxs);

	return 0;
}
s8
vfree(pageentry ptree[PAGE_TABLE_ENTRIES], uptr vaddr, uptr s)
{
	uptr i, lvlidxs[PAGE_TABLE_LEVELS] = PAGE_LVLIDXS_FROM_VADDR(vaddr);

	for (i = 0; i < CEIL(s, PAGE_SIZE) / PAGE_SIZE; i++) {
		pageentry *lastpt, *e;
		sptr l;
		void *f;

		/* Get the last-level page table containing the entry. */
		lastpt = ptree;
		for (l = 0; l < PAGE_TABLE_LEVELS - 1; l++)
			lastpt = (pageentry *)
			         PAGE_ENTRY_GET_PADDR(lastpt[lvlidxs[l]]);

		e = &lastpt[lvlidxs[PAGE_TABLE_LEVELS - 1]];

		f = (void *)PAGE_ENTRY_GET_PADDR(*e);

		if (pfree(f, PAGE_SIZE)) {
			tracepanicmsg("vfree");
			return -1;
		}

		/* Make the entry invalid. */
		*e = PAGE_ENTRY_REM_VALID(*e);

		/* Increase last-level index leading to entry and check:
		     1. if any page table is empty (i.e. full of invalid
		        entries): if it is, free it.
		     2. if any level index reaches PAGE_TABLE_ENTRIES: if it
		        does, increase the previous level index and reset the
		        current one to 0. */
		lvlidxs[PAGE_TABLE_LEVELS - 1]++;
		for (l = PAGE_TABLE_LEVELS - 1; l >= 0; l--) {
			sptr j;
			pageentry *pt;

			/* Check 1. */

			/* Find current page table pt. */
			pt = ptree;
			for (j = 0; j < l; j++)
				pt = (pageentry *)
				     PAGE_ENTRY_GET_PADDR(pt[lvlidxs[j]]);

			switch (walkpagetree(NULL, pt, 0, validentry, NULL)) {
			case -1:
				tracepanicmsg("vfree");
				return -1;
			case 1:
				/* Valid entry found: page table is not
				   empty. */
				break;
			default:
				/* Valid entry not found: page table is
				   empty. */
				if (pfree(pt, PAGE_TABLE_ENTRIES
				              * sizeof(pageentry))) {
					tracepanicmsg("vfree");
					return -1;
				}
			}

			/* Check 2. */

			if (lvlidxs[l] < PAGE_TABLE_ENTRIES)
				continue;

			if (!l) {
				setpanicmsg("End of page tree reached.");
				tracepanicmsg("vfree");
				return -1;
			}

			lvlidxs[l - 1]++;
			lvlidxs[l] = 0;
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
