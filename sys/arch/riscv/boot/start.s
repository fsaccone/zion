.section .entry.text
.globl _start
_start:
	li   t0, 0        # Hart ID to use
	csrr t1, mhartid  # Hart ID in use
	bne  t0, t1, lock # Lock non-0 harts

	la sp, _stack_end
	j  kmain

lock:
	wfi
	j lock
