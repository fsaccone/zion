#include "inittar.h"

#include <machine/mem.h>
#include <math.h>
#include <panic.h>
#include <pmem.h>
#include <tar.h>

/* At build time, the init.tar archive containing the init file system is
   embedded into the kernel binary right after this special magic sequence of
   bytes. It is important to not embed these bytes, in order, in a single
   structure, e.g. an array: the kernel may wrongly locate the init.tar archive
   at that location. */
#define INITTAR_MAGIC_BYTES_0  0x5a
#define INITTAR_MAGIC_BYTES_1  0x49
#define INITTAR_MAGIC_BYTES_2  0x4f
#define INITTAR_MAGIC_BYTES_3  0x4e
#define INITTAR_MAGIC_BYTES_4  0x49
#define INITTAR_MAGIC_BYTES_5  0x4e
#define INITTAR_MAGIC_BYTES_6  0x49
#define INITTAR_MAGIC_BYTES_7  0x54
#define INITTAR_MAGIC_BYTES_8  0x54
#define INITTAR_MAGIC_BYTES_9  0x41
#define INITTAR_MAGIC_BYTES_10 0x52
#define INITTAR_MAGIC_BYTES_11 0x53
#define INITTAR_MAGIC_BYTES_12 0x54
#define INITTAR_MAGIC_BYTES_13 0x41
#define INITTAR_MAGIC_BYTES_14 0x52
#define INITTAR_MAGIC_BYTES_15 0x54

/* Returns the address of the first tar header of init.tar or NULL if init.tar
   is not found in the kernel memory space. */
static struct tarheader *firsttarheader(void);

static struct tarheader *
firsttarheader(void)
{
	u8 *b;

	for (b = (u8 *)KERNEL_START;
	     (uptr)b < KERNEL_START + KERNEL_SIZE;
	     (uptr)b++) {
		if (b[0]  != INITTAR_MAGIC_BYTES_0
		 || b[1]  != INITTAR_MAGIC_BYTES_1
		 || b[2]  != INITTAR_MAGIC_BYTES_2
		 || b[3]  != INITTAR_MAGIC_BYTES_3
		 || b[4]  != INITTAR_MAGIC_BYTES_4
		 || b[5]  != INITTAR_MAGIC_BYTES_5
		 || b[6]  != INITTAR_MAGIC_BYTES_6
		 || b[7]  != INITTAR_MAGIC_BYTES_7
		 || b[8]  != INITTAR_MAGIC_BYTES_8
		 || b[9]  != INITTAR_MAGIC_BYTES_9
		 || b[10] != INITTAR_MAGIC_BYTES_10
		 || b[11] != INITTAR_MAGIC_BYTES_11
		 || b[12] != INITTAR_MAGIC_BYTES_12
		 || b[13] != INITTAR_MAGIC_BYTES_13
		 || b[14] != INITTAR_MAGIC_BYTES_14
		 || b[15] != INITTAR_MAGIC_BYTES_15)
			continue;

		return (struct tarheader *)((uptr)b + 16);
	}

	return NULL;
}

struct tarnode *
allocinittarfiles(void)
{
	struct tarheader *first, *h;
	struct tarnode *head = NULL, *tail;

	if (!(first = firsttarheader()))
		return NULL;

	for (h = first;
	     (uptr)h < KERNEL_START + KERNEL_SIZE && istarheader(h);
	     h = (struct tarheader *)((uptr)h + sizeof(struct tarheader))) {
		uptr s;
		struct tarnode *new;

		if (!(new = palloc(sizeof(struct tarnode), 0))) {
			tracepanicmsg("allocinittarfiles");
			panic();
		}

		new->h = h;

		if (!head)
			head = new;

		tail->n = new;
		tail = new;

		/* Skip content sectors. */
		if ((s = tarsize(h)))
			h += CEIL(s, sizeof(struct tarheader));
	}

	return head;
}

void
freeinittarfiles(struct tarnode *files)
{
	struct tarnode *f, *n;

	for (f = files; f; f = n) {
		n = f->n;
		pfree(f, sizeof(struct tarnode));
	}
}
