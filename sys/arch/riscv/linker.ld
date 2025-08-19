OUTPUT_FORMAT(elf32-littleriscv)
OUTPUT_ARCH(riscv:rv32)
ENTRY(_start)

MEMORY
{
	ROM (rx)  : ORIGIN = 0x80000000, LENGTH = 0x10000
	RAM (rwx) : ORIGIN = 0x80010000, LENGTH = 32M
}

SECTIONS
{
	. = ORIGIN(RAM);

	.text : {
		*(.boot.text)
		*(.text)
	} >ROM

	.rodata : {
		*(.boot.rodata)
		*(.rodata)
	} >ROM

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
	} >RAM
}
