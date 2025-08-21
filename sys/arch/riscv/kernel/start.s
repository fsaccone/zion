.section .entry.text
.globl _start

_start:
	li   a0, 0
	la   a1, zero_ctx
	call switch_ctx

	la   t0,    machine_trap_vector
	csrw mtvec, t0

	la   t0,    supervisor_trap_vector
	csrw stvec, t0

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

machine_trap_vector:
	csrr a0, mcause
	call handle_trap
	mret

supervisor_trap_vector:
	csrr a0, scause
	call handle_trap
	sret

.section .rodata
.align 4
zero_ctx: .zero 127
