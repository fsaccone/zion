#include <pagetable.h>

#include <arch/page.h>
#include <arch/types.h>
#include <pmem.h>

/* To use with walkpagetree. Returns 1 if the value in ptr is equal to e or 0
   otherwise. */
static u8 equalentries(pageentry e, void *ptr);

/* To use with walkpagetree. Returns 1 if entry e is invalid or 0 otherwise. */
static u8 invalidentry(pageentry e, void *);

/* To use with walkpagetree. Returns 1 if entry e is valid or 0 otherwise. */
static u8 validentry(pageentry e, void *);

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


u8
validentry(pageentry e, void *)
{
	return PAGE_ENTRY_GET_VALID(e);
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
