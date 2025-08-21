#include <usermem.h>

#include <arch/mem.h>
#include <arch/types.h>
#include <log.h>

#define PAGE_CEIL(x)  (((x) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))
#define PAGE_FLOOR(x) ((x) & ~(PAGE_SIZE - 1))

struct page {
	struct page *n;
};

extern void *_memory_end;

static struct page *freepages;

void
usermem_free(void *ptr)
{
	struct page *p;

	if ((int)ptr % PAGE_SIZE)
		panic("Invalid address passed to free.");

	if (ptr >= _memory_end)
		panic("Restricted address passed to free.");

	usermem_memset(ptr, 0, PAGE_SIZE);

	p = ptr;

	/* Append freed page to freepages linked list */
	p->n = freepages;
	freepages = p;
}

void
usermem_freeall(void)
{
	int p;

	/* Free all memory */
	for (p = 0; p <= (int)_memory_end - PAGE_SIZE; p += PAGE_SIZE)
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
