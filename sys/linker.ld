ENTRY(_start)

MEMORY
{
	RAM (wxa!ri) : ORIGIN = 0x80000000, LENGTH = 32M
}

SECTIONS
{
	. = ORIGIN(RAM);

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
