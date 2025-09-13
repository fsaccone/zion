#include <pmem.h>

#include <arch/page.h>
#include <arch/types.h>
#include <log.h>
#include <math.h>

struct frame {
	struct frame *n;
};

/* Removes p from freeframes. */
static void allocframe(struct frame *f);

/* Finds p in freeframes and returns it as a struct frame *. */
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
		if ((un)f == (un)ff)
			return ff;
	}

	return NULL;
}

void *
palloc(un s)
{
	struct frame *first;
	struct frame *frames[MAX_PALLOC_FRAMES];
	un i, nframes = CEIL(s, PAGE_SIZE) / PAGE_SIZE;

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

	for (i = 0; i < nframes; i++) {
		frames[i] = freeframe((void *)((un)first - i * PAGE_SIZE));

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
	while (!palloc(PAGE_SIZE));
}

int
pfree(void *f, un s)
{
	un i;

	if ((un)f % PAGE_SIZE) {
		setpanicmsg("Misaligned frame.");
		tracepanicmsg("pfree");
		return -1;
	}

	for (i = 0; i < CEIL(s, PAGE_SIZE) / PAGE_SIZE; i++) {
		struct frame *q = (struct frame *)((un)f + i * PAGE_SIZE);

		q->n = freeframes;
		freeframes = q;
	}

	return 0;
}

int
pfreerange(void *s, void *e)
{
	void *p;

	if (CEIL((un)s, PAGE_SIZE) >= (un)e) {
		setpanicmsg("No frames in range.");
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
