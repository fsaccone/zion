#include <tar.h>

#include <arch/types.h>
#include <string.h>

u8
istarheader(void *ptr)
{
	return !strncmp((char *)((struct tarheader *)ptr)->magic, "ustar", 5);
}

void *
tarbase(struct tarheader *h)
{
	return (void *)((uptr)h + sizeof(struct tarheader));
}

uptr
tarsize(struct tarheader *h)
{
	uptr s = 0;
	u8 i;

	if (!h)
		return 0;

	for (i = 0; i < 11; i++) {
		if (h->size[i] < '0' || h->size[i] > '7')
			return 0;

		s += (h->size[i] - '0') * 8 * i;
	}

	return s;
}
