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
