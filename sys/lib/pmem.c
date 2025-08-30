#include <pmem.h>

#include <arch.h>
#include <log.h>

#define PAGE_CEIL(a)  (void *)(((uintn_t)(a)) & ~(PAGE_SIZE - 1))
#define PAGE_FLOOR(a) (void *)((((uintn_t)(a)) + PAGE_SIZE - 1) \
                               & ~(PAGE_SIZE - 1))

struct page {
	struct page *n;
};

struct page *freepages;

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

	if (PAGE_CEIL(s) >= e)
		panic("pfreerange: no pages in range");

	for (p = PAGE_CEIL(s);
	     (uintn_t)p + PAGE_SIZE <= (uintn_t)e;
	     p = (void *)((uintn_t)p + PAGE_SIZE))
		pfree(p);
}
