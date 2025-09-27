#include <elf.h>

#include <arch/types.h>

struct elffileheader {
	u32  magic;
	u8   class;
	u8   data;
	u8   version;
	u8   osabi;
	u64  reserved;
	u16  type;
	u16  isa;
	u32  elfversion;
	uptr peoff;
	uptr phoff;
	uptr shoff;
	u32  flags;
	u16  phesize;
	u16  shesize;
	u16  shstridx;
};

void *
allocelf(void *elf)
{
	/* (TODO) */
	return elf;
}
