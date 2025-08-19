ENTRY(_start)

MEMORY
{
	RAM (wxa!ri) : ORIGIN = 0x80000000, LENGTH = 32M
}

SECTIONS
{
	. = ORIGIN(RAM);

	.text : {
		*(.boot.text)
		*(.text)
	} >RAM

	.rodata : {
		*(.boot.rodata)
		*(.rodata)
	} >RAM

	.data : {
		*(.boot.data)
		*(.data)
	} >RAM

	.bss : {
		*(.boot.bss)
		*(.bss)
	} >RAM
}
