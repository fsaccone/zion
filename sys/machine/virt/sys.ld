ENTRY(_entry)

MEMORY
{
	# Qemu's virt machine loads the kernel at this starting address
	ROM (rx)  : ORIGIN = 0x80000000,        LENGTH = 256K
	RAM (rwx) : ORIGIN = 0x80000000 + 256K, LENGTH = 1M
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
}
