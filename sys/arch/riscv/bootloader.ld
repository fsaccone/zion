ENTRY(_entry)

MEMORY
{
	ROM (rx)  : ORIGIN = 0,       LENGTH = 0x80000
	RAM (rwx) : ORIGIN = 0x80000, LENGTH = 0x20000
}

SECTIONS
{
	.mbr : {
		*(.entry.text)
		. = ALIGN(510);
		BYTE(0x55);
		BYTE(0xaa);
	} > ROM

	.text : {
		*(.text)
	} > ROM

	.rodata : {
		*(.rodata)
	} > ROM

	.data : {
		*(.data)
	} > RAM

	.bss : {
		_bss_start = .;
		*(.bss)
		*(COMMON)
		_bss_end = .;
	} > RAM

	.stack (NOLOAD) : {
		. = ALIGN(8);
		_stack_start = .;
		. = . + 0x1000;
		_stack_end = .;
	} > RAM
}
