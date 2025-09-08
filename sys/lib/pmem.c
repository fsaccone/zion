#include <pmem.h>

#include <arch/page.h>
#include <arch/types.h>
#include <log.h>
#include <math.h>

struct page {
	struct page *n;
};

/* Removes p from freepages */
static void allocpage(struct page *p);

/* Finds p in freepages and returns it as a struct page * */
static struct page *freepage(void *p);

static struct page *freepages = NULL;

void
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

struct page *
freepage(void *p)
{
	struct page *q;

	for (q = freepages; q; q = q->n) {
		if ((un)p == (un)q)
			return p;
	}

	return NULL;
}

void *
palloc(un s)
{
	struct page *first;
	struct page *pages[MAX_PALLOC_FRAMES];
	un i, npages = CEIL(s, PAGE_SIZE) / PAGE_SIZE;

	if (!s || s > MAX_PALLOC) {
		setpanicmsg("Invalid size.");
		tracepanicmsg("palloc");
		return NULL;
	}

	first = freepages;
nextfirst:
	if (!first) {
		setpanicmsg("Memory full.");
		tracepanicmsg("palloc");
		return NULL;
	}

	for (i = 0; i < npages; i++) {
		pages[i] = freepage((void *)((un)first - i * PAGE_SIZE));

		if (!pages[i]) {
			first = first->n;
			goto nextfirst;
		}
	}

	/* Since the search happens from the highest to the lowest address,
	   first is needed to be set to the last element of pages */
	first = pages[npages - 1];

	for (i = 0; i < npages; i++) {
		allocpage(pages[i]);
		pmemset(pages[i], 0, PAGE_SIZE);
	}

	return first;
}

int
pfree(void *f, un s)
{
	un i;

	if ((un)f % PAGE_SIZE) {
		setpanicmsg("Misaligned page.");
		tracepanicmsg("pfree");
		return -1;
	}

	for (i = 0; i < CEIL(s, PAGE_SIZE) / PAGE_SIZE; i++) {
		struct page *q = (struct page *)((un)f + i * PAGE_SIZE);

		q->n = freepages;
		freepages = q;
	}

	return 0;
}

int
pfreerange(void *s, void *e)
{
	void *p;

	if (CEIL((un)s, PAGE_SIZE) >= (un)e) {
		setpanicmsg("No pages in range.");
		tracepanicmsg("pfreerange");
		return -1;
	}

	for (p = (void *)CEIL((un)s, PAGE_SIZE);
	     (un)p + PAGE_SIZE <= (un)e;
	     p = (void *)((un)p + PAGE_SIZE)) {
		if (pfree(p, PAGE_SIZE)) {
			tracepanicmsg("pfreerange");
			return -1;
		}
	}

	return 0;
}

void
pmemset(void *s, u8 c, un n)
{
	while (n--)
		*(u8 *)s++ = c;
}
