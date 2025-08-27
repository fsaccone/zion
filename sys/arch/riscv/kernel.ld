ENTRY(_entry)

MEMORY
{
	# Qemu's virt machine loads the kernel at this starting address
	ROM (rx)  : ORIGIN = 0x80000000, LENGTH = 0x80000
	RAM (rwx) : ORIGIN = 0x80080000, LENGTH = 0x20000
}

SECTIONS
{
	.text : {
		*(.entry.text)
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

	.heap (NOLOAD) : {
		. = ALIGN(8);
		_heapstart = .;
		. = . + 0x1000;
		_heapend = .;
	} > RAM
}
