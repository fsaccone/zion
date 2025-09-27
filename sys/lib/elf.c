#include <elf.h>

#include <arch/page.h>
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
	u16  ehsize;
	u16  phesize;
	u16  phenum;
	u16  shesize;
	u16  shenum;
	u16  shstridx;
};

struct elfprogheader {
	u32  stype;
#ifdef ELF_BITS_64
	u32  flags;
#endif
	uptr soff;
	uptr svaddr;
	uptr spaddr;
	uptr sfsize;
	uptr smsize;
#ifdef ELF_BITS_32
	u32  flags;
#endif
	uptr alignment;
};

void *
allocelf(void *elf, pageentry *pt[PAGE_TABLE_ENTRIES])
{
	/* (TODO) */
	(void)pt;
	return elf;
}
