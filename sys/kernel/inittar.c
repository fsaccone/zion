#include "inittar.h"

#include <inittar.h>
#include <machine/mem.h>
#include <pmem.h>
#include <tar.h>

static u8 inittarmagic[INITTAR_MAGIC_BYTES_LENGTH] = INITTAR_MAGIC_BYTES;

struct tarheader *
inittaraddress(void)
{
	u8 *b;

	for (b = (u8 *)KERNEL_START;
	     (uptr)b < KERNEL_START + KERNEL_SIZE;
	     (uptr)b++) {
		u8 i, found = 1;

		for (i = 0; i < INITTAR_MAGIC_BYTES_LENGTH; i++) {
			if (b[i] != inittarmagic[i]) {
				found = 0;
				break;
			}
		}

		if (!found)
			continue;

		return (struct tarheader *)
		       ((uptr)b + INITTAR_MAGIC_BYTES_LENGTH);
	}

	return NULL;
}
