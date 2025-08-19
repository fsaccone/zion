.section .boot.text
.globl _start
_start:
	li   t0, 0        # Hart ID to use
	csrr t1, mhartid  # Hart ID in use
	bne  t0, t1, loop # Disable non-0 harts

	la sp, _stack_end
	j  kmain

loop:
	wfi
	j loop
