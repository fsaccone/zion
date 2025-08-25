.section .entry.text
.globl _entry

_entry:
	call initialize_misa
	call lock_harts
	call clear_bss
	call clear_registers

	la sp, _stack_end

	j bmain

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

lock_harts:
	li   t0, 0
	csrr t1, mhartid
	bne  t0, t1, lock

	ret

lock:
	wfi
	j lock
