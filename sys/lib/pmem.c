#include <pmem.h>

#include <arch.h>
#include <log.h>
#include <math.h>

struct page {
	struct page *n;
};

struct page *freepages = NULL;

static void *
memset(void *s, int c, size_t n)
{
	int *p = s;

	while (n--)
		*p++ = c;

	return p;
}

void *
palloc(void)
{
	struct page *p;

	p = freepages;
	if (p) {
		freepages = p->n;
		memset(p, 0, PAGE_SIZE);
	}

	return p;
}

void
pfree(void *p)
{
	struct page *ap;

	if ((uintn_t)p % PAGE_SIZE)
		panic("pfree: misaligned page");

	ap = p;

	ap->n = freepages;
	freepages = ap;
}

void
pfreerange(void *s, void *e)
{
	void *p;

	if (CEIL((uintn_t)s, PAGE_SIZE) >= (uintn_t)e)
		panic("pfreerange: no pages in range");

	for (p = (void *)CEIL((uintn_t)s, PAGE_SIZE);
	     (uintn_t)p + PAGE_SIZE <= (uintn_t)e;
	     p = (void *)((uintn_t)p + PAGE_SIZE))
		pfree(p);
}
