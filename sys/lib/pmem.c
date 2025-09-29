#include <pmem.h>

#include <arch/page.h>
#include <arch/types.h>
#include <math.h>
#include <panic.h>

struct frame {
	struct frame *n;
};

/* Removes f from freeframes. */
static void allocframe(struct frame *f);

/* Finds f in freeframes and returns it as a struct frame *. */
static struct frame *freeframe(void *f);

static struct frame *freeframes = NULL;

void
allocframe(struct frame *f)
{
	struct frame *prev;

	if (!f)
		return;

	if (freeframes == f) {
		freeframes = freeframes->n;
		return;
	}

	/* Find prev freeframes element which points to f. */
	for (prev = freeframes; prev->n != f; prev = prev->n) {
		if (!prev)
			return;
	}

	prev->n = f->n;
}

struct frame *
freeframe(void *f)
{
	struct frame *ff;

	for (ff = freeframes; ff; ff = ff->n) {
		if ((uptr)f == (uptr)ff)
			return ff;
	}

	return NULL;
}

void *
palloc(uptr s, uptr align)
{
	struct frame *first;
	struct frame *frames[MAX_PALLOC_FRAMES];
	uptr i, nframes = CEIL(s, PAGE_SIZE) / PAGE_SIZE;

	if (!align)
		align = PAGE_SIZE;

	if (align % PAGE_SIZE) {
		setpanicmsg("Invalid alignment.");
		tracepanicmsg("palloc");
		return NULL;
	}

	if (!s || s > MAX_PALLOC) {
		setpanicmsg("Invalid size.");
		tracepanicmsg("palloc");
		return NULL;
	}

	first = freeframes;
nextfirst:
	if (!first) {
		setpanicmsg("Memory full.");
		tracepanicmsg("palloc");
		return NULL;
	}

	/* If first page is unaligned. */
	if ((uptr)first % align) {
		first = first->n;
		goto nextfirst;
	}

	for (i = 0; i < nframes; i++) {
		frames[i] = freeframe((void *)((uptr)first - i * PAGE_SIZE));

		if (!frames[i]) {
			first = first->n;
			goto nextfirst;
		}
	}

	/* Since the search happens from the highest to the lowest address,
	   first is needed to be set to the last element of frames. */
	first = frames[nframes - 1];

	for (i = 0; i < nframes; i++) {
		allocframe(frames[i]);
		pmemset(frames[i], 0, PAGE_SIZE);
	}

	return first;
}

void
pcleanup(void)
{
	while (!palloc(PAGE_SIZE, 0));
}

s8
pfree(void *f, uptr s)
{
	uptr i;

	if ((uptr)f % PAGE_SIZE) {
		setpanicmsg("Misaligned frame.");
		tracepanicmsg("pfree");
		return -1;
	}

	for (i = 0; i < CEIL(s, PAGE_SIZE) / PAGE_SIZE; i++) {
		struct frame *q = (struct frame *)((uptr)f + i * PAGE_SIZE);

		q->n = freeframes;
		freeframes = q;
	}

	return 0;
}

void
pmemcpy(void *d, void *s, uptr n)
{
	while (n--)
		*(u8 *)d++ = *(u8 *)s++;
}

void
pmemset(void *s, u8 c, uptr n)
{
	while (n--)
		*(u8 *)s++ = c;
}
