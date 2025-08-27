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
		_bssstart = .;
		*(.bss)
		*(COMMON)
		_bssend = .;
	} > RAM

	.stack (NOLOAD) : {
		. = ALIGN(8);
		_stackstart = .;
		. = . + 0x1000;
		_stackend = .;
	} > RAM
}
