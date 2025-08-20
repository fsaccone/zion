OUTPUT_FORMAT(elf32-littleriscv)
OUTPUT_ARCH(riscv:rv32)
ENTRY(_start)

MEMORY
{
	rom (rx)   : ORIGIN = 0x80000000, LENGTH = 0x10000
	ram (rwxa) : ORIGIN = 0x80010000, LENGTH = 32M
}

SECTIONS
{
	. = ORIGIN(ram);

	.text : {
		*(.boot.text)
		*(.text)
	} >rom AT>rom

	.rodata : {
		*(.boot.rodata)
		*(.rodata)
	} >rom AT>rom

	.data : {
		*(.boot.data)
		*(.data)
	} >ram AT>ram

	.bss : {
		*(.boot.bss)
		*(.bss)
	} >ram AT>ram

	.stack (NOLOAD) : {
		. = ALIGN(8);
		_stack_start = .;
		. = . + 0x1000;
		_stack_end = .;
	} >ram AT>ram

	.heap (NOLOAD) : {
		. = ALIGN(8);
		_heap_start = .;
		. = . + 0x1000;
		_heap_end = .;
	} >ram AT>ram
}
