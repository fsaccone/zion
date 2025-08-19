OUTPUT_FORMAT(elf32-littleriscv)
OUTPUT_ARCH(riscv:rv32)
ENTRY(_start)

MEMORY
{
	RAM (rwx) : ORIGIN = 0x80000000, LENGTH = 32M
}

SECTIONS
{
	. = ORIGIN(RAM);

	.text : {
		*(.boot.text)
		*(.text)
	} >RAM

	.rodata : {
		*(.boot.rodata)
		*(.rodata)
	} >RAM

	.data : {
		*(.boot.data)
		*(.data)
	} >RAM

	.bss : {
		*(.boot.bss)
		*(.bss)
	} >RAM

	.stack (NOLOAD) : {
		. = ALIGN(8);
		_stack_start = .;
		. = . + 0x1000;
		_stack_end = .;
	}
}
