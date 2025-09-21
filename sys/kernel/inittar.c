#include "inittar.h"

#include <inittar.h>
#include <machine/mem.h>
#include <pmem.h>
#include <tar.h>

struct tarheader *
inittaraddress(void)
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
