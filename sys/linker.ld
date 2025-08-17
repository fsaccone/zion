ENTRY(_start)

SECTIONS
{
	.text : {
		*(.text)
	}

	.bss : {
		*(.bss)
	}
}
