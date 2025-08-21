.section .entry.text
.globl _start

_start:
	li   a0, 0
	la   a1, zero_ctx
	call switch_ctx

	call initialize_misa
	call initialize_vectors
	call lock_harts

	la sp, _stack_end

	j kmain

initialize_misa:
	li   t0, 0

	# RV32I/64I base ISA
	li  t1, 1 << 8
	add t0, t0, t1

	# Supervisor mode implemented
	li  t1, 1 << 18
	add t0, t0, t1

	# User mode implemented
	li  t1, 1 << 20
	add t0, t0, t1

	csrw misa, t0

	ret

initialize_vectors:
	la   t0,    machine_trap_vector
	csrw mtvec, t0

	la   t0,    supervisor_trap_vector
	csrw stvec, t0

	ret

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
