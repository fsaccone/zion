.section .bss
.align 12
.space 0x4000
stacktop:

.section .text
.globl _start
_start:
	la sp, stacktop
	call kmain
	j loop
loop:
	j loop
