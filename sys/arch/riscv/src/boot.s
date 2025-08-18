.section .text
.globl _start
_start:
	li t0, 0         # Hart ID to use
	csrr t1, mhartid # Hart ID in use
	bne t0, t1, loop # Disable non-0 harts

	la sp, stacktop
	j kmain

loop:
	j loop

.section .bss
.align 12
.space 4096
stacktop:
