#include <pmem.h>

#include <arch.h>
#include <log.h>
#include <math.h>

struct page {
	struct page *n;
};

struct page *freepages = NULL;

void *
palloc(uintn s)
{
	struct page *p;
	uintn i, npages = CEIL(s, PAGE_SIZE) / PAGE_SIZE;

	if (!s)
		return NULL;

	p = freepages;
	for (i = 0; i < npages; i++) {
		if (!p)
			return NULL;

		freepages = p->n;
		pmemset(p, 0, PAGE_SIZE);
		p = freepages;
	}

	return p;
}

void
pfree(void *p)
{
	struct page *ap;

	if ((uintn)p % PAGE_SIZE)
		panic("pfree: misaligned page");

	ap = p;

	ap->n = freepages;
	freepages = ap;
}

void
pfreerange(void *s, void *e)
{
	void *p;

	if (CEIL((uintn)s, PAGE_SIZE) >= (uintn)e)
		panic("pfreerange: no pages in range");

	for (p = (void *)CEIL((uintn)s, PAGE_SIZE);
	     (uintn)p + PAGE_SIZE <= (uintn)e;
	     p = (void *)((uintn)p + PAGE_SIZE))
		pfree(p);
}

void *
pmemset(void *s, int c, uintn n)
{
	int *p = s;

	while (n--)
		*p++ = c;

	return p;
}
