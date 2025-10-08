#include <pmem.h>

#include <arch/page.h>
#include <arch/types.h>
#include <math.h>
#include <panic.h>
#include <spinlock.h>

struct freeframe {
	struct freeframe *n;
};

/* Removes f from freeframes. */
static void allocframe(struct freeframe *f);

/* Finds f in freeframes and returns it as a struct freeframe *. */
static struct freeframe *freeframe(void *f);

static struct freeframe *freeframes = NULL;
static struct lock l;

void
allocframe(struct freeframe *f)
{
	struct freeframe *prev;

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

struct freeframe *
freeframe(void *f)
{
	struct freeframe *ff;

	for (ff = freeframes; ff; ff = ff->n) {
		if ((uptr)f == (uptr)ff)
			return ff;
	}

	return NULL;
}

void *
palloc(uptr s, uptr align)
{
	struct freeframe *first;
	struct freeframe *frames[MAX_PALLOC_FRAMES];
	uptr i, nframes = CEIL(s, PAGE_SIZE) / PAGE_SIZE;

	if (!align)
		align = PAGE_SIZE;

	if (align % PAGE_SIZE) {
		setpanicmsg("Invalid alignment.");
		goto panic;
	}

	if (!s || s > MAX_PALLOC) {
		setpanicmsg("Invalid size.");
		goto panic;
	}

	lock(&l);

	first = freeframes;
nextfirst:
	if (!first) {
		setpanicmsg("Memory full.");
		goto panic;
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

	unlock(&l);

	return first;

panic:
	tracepanicmsg("palloc");
	return NULL;
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
		goto panic;
	}

	lock(&l);

	for (i = 0; i < CEIL(s, PAGE_SIZE) / PAGE_SIZE; i++) {
		struct freeframe *q = (struct freeframe *)
		                      ((uptr)f + i * PAGE_SIZE);

		q->n = freeframes;
		freeframes = q;
	}

	unlock(&l);

	return 0;

panic:
	tracepanicmsg("pfree");
	return -1;
}

s8
pmemcmp(void *a, void *b, uptr n)
{
	for (; n > 0; n--, a++, b++) {
		if (*(u8 *)a != *(u8 *)b)
			return *(u8 *)a - *(u8 *)b;
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
