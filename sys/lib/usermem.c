#include <usermem.h>

#include <arch/mem.h>
#include <arch/types.h>
#include <log.h>

#define PAGE_CEIL(x)  (((x) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))
#define PAGE_FLOOR(x) ((x) & ~(PAGE_SIZE - 1))

struct page {
	struct page *n;
};

static struct page *freepages;
static void *memend;

void *
usermem_alloc(size_t s)
{
	int i, pages = PAGE_CEIL(s) / PAGE_SIZE;
	struct page *p = freepages;

	if (!p)
		panic("Unable to allocate memory.");

	for (i = 0; i < pages; i++)
		freepages = freepages->n;

	return (void *)p;
}

void
usermem_free(void *ptr)
{
	struct page *p;

	if (!memend)
		panic("Called free before usermem_init.");

	if ((int)ptr % PAGE_SIZE)
		panic("Invalid address passed to free.");

	if (ptr >= memend)
		panic("Restricted address passed to free.");

	usermem_memset(ptr, 0, PAGE_SIZE);

	p = ptr;

	/* Append freed page to freepages linked list */
	p->n = freepages;
	freepages = p;
}

void
usermem_init(void *kstart)
{
	int p;

	memend = (void *)PAGE_FLOOR((int)kstart - 1);

	/* Free all memory */
	for (p = 0; p <= PAGE_FLOOR((int)kstart); p += PAGE_SIZE)
		usermem_free((void *)p);
}

void *
usermem_memset(void *s, int c, size_t n)
{
	void *r = s;

	while (n--)
		*(int *)s++ = c;

	return r;
}
