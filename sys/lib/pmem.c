#include <pmem.h>

#include <arch.h>
#include <log.h>
#include <math.h>

struct page {
	struct page *n;
};

/* Removes p from freepages */
static void allocpage(struct page *p);

/* Finds p in freepages and returns it as a struct page * */
static struct page *freepage(void *p);

struct page *freepages = NULL;

static void
allocpage(struct page *p)
{
	struct page *prev;

	if (!p)
		return;

	if (freepages == p) {
		freepages = freepages->n;
		return;
	}

	/* Find prev freepages element which points to p */
	for (prev = freepages; prev->n != p; prev = prev->n) {
		if (!prev)
			return;
	}

	prev->n = p->n;
}

static struct page *
freepage(void *p)
{
	struct page *q;

	for (q = freepages; q; q = q->n) {
		if ((uintn)p == (uintn)q)
			return p;
	}

	return NULL;
}

void *
palloc(uintn s)
{
	struct page *first;
	struct page *pages[MAX_PALLOC_FRAMES];
	uintn i, npages = CEIL(s, PAGE_SIZE) / PAGE_SIZE;

	if (!s || s > MAX_PALLOC)
		return NULL;

	first = freepages;
nextfirst:
	if (!first)
		return NULL;

	for (i = 0; i < npages; i++) {
		pages[i] = freepage((void *)((uintn)first - i * PAGE_SIZE));

		if (!pages[i]) {
			first = first->n;
			goto nextfirst;
		}
	}

	for (i = 0; i < npages; i++) {
		allocpage(pages[i]);
		pmemset(pages[i], 0, PAGE_SIZE);
	}

	return first;
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

void
pmemset(void *s, int c, uintn n)
{
	while (n--)
		*(int *)s++ = c;
}
