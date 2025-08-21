.section .entry.text
.globl _start

_start:
	li   a0, 0
	la   a1, zero_ctx
	call switch_ctx

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

.section .rodata
.align 4
zero_ctx: .zero 127
