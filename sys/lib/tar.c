#include <tar.h>

#include <arch/types.h>

u8
istarheader(void *ptr)
{
	u8 *m = ((struct tarheader *)ptr)->magic;

	if (m[0] == 'u'
	 && m[1] == 's'
	 && m[2] == 't'
	 && m[3] == 'a'
	 && m[4] == 'r')
		return 1;

	return 0;
}

uptr
tarheadersize(struct tarheader *h)
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
