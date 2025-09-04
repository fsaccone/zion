.section .entry.text
.global _entry

_entry:
	la sp, _stackend

	call initmisa
	call initvecs
	call lockharts

	call setmstatus
	call setsstatus

	j supervisor

initmisa:
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

initvecs:
	la   t0,    mtrapvec
	csrw mtvec, t0

	la   t0,    strapvec
	csrw stvec, t0

	ret

lockharts:
	li   t0, 0
	csrr tp, mhartid
	bne  t0, tp, 1f
	ret
1:
	wfi
	j 1b

mtrapvec:
	csrr a0, mcause
	call interrupt
	mret

strapvec:
	csrr a0, scause
	call interrupt
	sret

supervisor:
	la   t0,   callkmain
	csrw mepc, t0

	li   t0,      0xffffffffffffffff
	csrw medeleg, t0
	csrw mideleg, t0

	# Kernel PMP address range (all addresses)
	li   t0,       0x00000000
	li   t1,       0xffffffffffffffff
	csrw pmpaddr0, t0
	csrw pmpaddr1, t1

	# Kernel PMP configuration
	li   t0,      0
	li   t1,      1 << 0 # Read permission
	or   t0,      t0, t1
	li   t1,      1 << 1 # Write permission
	or   t0,      t0, t1
	li   t1,      1 << 2 # Execute permission
	or   t0,      t0, t1
	li   t1,      1 << 3 # TOR address-matching mode
	or   t0,      t0, t1
	li   t1,      1 << 7 # Lock
	or   t0,      t0, t1
	slli t0,      t0, 8  # Write to pmp1cfg
	csrw pmpcfg0, t0

	mret

callkmain:
	j kmain

.section .rodata
.align 4
zeroctx: .zero 127
