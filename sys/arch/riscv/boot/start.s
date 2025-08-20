.section .entry.text
.globl _start
_start:
	call lock_harts

	la sp, _stack_end
	j  kmain

lock_harts:
	li   t0, 0
	csrr t1, mhartid
	bne  t0, t1, lock

	ret

lock:
	wfi
	j lock
