.section .text
.globl _start
_start:
	la sp, stacktop
	j kmain

.section .bss
.align 12
.space 4096
stacktop:
