ENTRY(_start)

MEMORY
{
	RAM (wxa!ri) : ORIGIN = 0x80000000, LENGTH = 32M
}

SECTIONS
{
	. = ORIGIN(RAM);

	.boot.text : {
		*(.boot.text)
	} >RAM

	.boot.rodata : {
		*(.boot.rodata)
	} >RAM

	.boot.data : {
		*(.boot.data)
	} >RAM

	.boot.bss : {
		*(.boot.bss)
	} >RAM

	.text : {
		*(.text)
	} >RAM

	.rodata : {
		*(.rodata)
	} >RAM

	.data : {
		*(.data)
	} >RAM

	.bss : {
		*(.bss)
	} >RAM
}
