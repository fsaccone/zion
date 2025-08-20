OUTPUT_FORMAT(elf32-littleriscv)
OUTPUT_ARCH(riscv:rv32)
ENTRY(_start)

MEMORY
{
	rom (rx)  : ORIGIN = 0x80000000, LENGTH = 0x10000
	ram (rwx) : ORIGIN = 0x80010000, LENGTH = 32M
}

SECTIONS
{
	. = ORIGIN(ram);

	.text : {
		*(.boot.text)
		*(.text)
	} >rom

	.rodata : {
		*(.boot.rodata)
		*(.rodata)
	} >rom

	.data : {
		*(.boot.data)
		*(.data)
	} >ram

	.bss : {
		*(.boot.bss)
		*(.bss)
	} >ram

	.stack (NOLOAD) : {
		. = ALIGN(8);
		_stack_start = .;
		. = . + 0x1000;
		_stack_end = .;
	} >ram

	.heap (NOLOAD) : {
		. = ALIGN(8);
		_heap_start = .;
		. = . + 0x1000;
		_heap_end = .;
	} >ram
}
