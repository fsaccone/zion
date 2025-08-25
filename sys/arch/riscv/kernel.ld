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
		*(.bss)
		*(COMMON)
	} > RAM

	.stack (NOLOAD) : {
		. = ALIGN(8);
		_stack_start = .;
		. = . + 0x1000;
		_stack_end = .;
	} > RAM

	.heap (NOLOAD) : {
		. = ALIGN(8);
		_heap_start = .;
		. = . + 0x1000;
		_heap_end = .;
	} > RAM
}
