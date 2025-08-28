#include <pmem.h>

#include <arch.h>
#include <log.h>

struct node {
	struct node *n;
};

struct node *freepages;

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
	struct node *n;

	n = freepages;
	if (n) {
		freepages = n->n;
		memset(n, 0, PAGE_SIZE);
	}

	return n;
}

void
pfree(void *p)
{
	struct node *n;

	if ((uintn_t)p % PAGE_SIZE)
		panic("pfree");

	n = p;

	n->n = freepages;
	freepages = n;
}
