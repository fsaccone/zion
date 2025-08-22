OUTPUT_FORMAT(elf32-littleriscv)
OUTPUT_ARCH(riscv:rv32)
ENTRY(_start)

SECTIONS
{
	.text : {
		*(.entry.text)
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

	.heap (NOLOAD) : {
		. = ALIGN(8);
		_heap_start = .;
		. = . + 0x1000;
		_heap_end = .;
	}
}
