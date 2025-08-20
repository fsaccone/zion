.section .entry.text
.globl _start

_start:
	la sp, _stack_end

	call lock_harts

	j  kmain

lock_harts:
	li   t0, 0
	csrr t1, mhartid
	bne  t0, t1, lock

	ret

lock:
	wfi
	j lock
