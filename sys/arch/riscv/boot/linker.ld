OUTPUT_FORMAT(elf32-littleriscv)
OUTPUT_ARCH(riscv:rv32)
ENTRY(_start)

SECTIONS
{
	. = 0;
	
	_memory_start = .;

	.mbr : {
		*(.mbr)
		. = ALIGN(510);
		BYTE(0x55);
		BYTE(0xaa);
	}

	.text : {
		*(.text)
	}

	.rodata : {
		*(.rodata)
	}

	.data : {
		*(.data)
	}

	.bss : {
		*(.bss)
		*(COMMON)
	}

	.stack (NOLOAD) : {
		. = ALIGN(8);
		_stack_start = .;
		. = . + 0x1000;
		_stack_end = .;
	}

	_memory_end = .;
}
