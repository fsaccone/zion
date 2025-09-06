.section .text
.global astart

astart:
	la sp, stackend

	call initmisa
	call initvecs
	call lockharts

	call initmstatus
	call initsstatus

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

initmstatus:
	csrr t0, mstatus

	# Set SIE to 1
	li t1, 1 << 1
	or t0, t0, t1

	# Set MIE to 1
	li t1, 1 << 3
	or t0, t0, t1

	# Set MPP[0] to 1
	li t1, 1 << 11
	or t0, t0, t1

	# Set MPP[1] to 0 (MPP = 0b01)
	li  t1, 1 << 12
	not t1, t1
	and t0, t0, t1

	# Set MPIE to 0
	li  t1, 1 << 7
	not t1, t1
	and t0, t0, t1

	csrw mstatus, t0

	ret

initsstatus:
	csrr t0, sstatus

	# Set SUM to 1
	li t1, 1 << 18
	or t0, t0, t1

	# Set MXR to 1
	li t1, 1 << 19
	or t0, t0, t1

	csrw sstatus, t0

	ret

lockharts:
	li   t0, 0
	csrr tp, mhartid
	bne  t0, tp, 1f
	ret
1:
	wfi
	j 1b

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

.section .data

stackend: .space 4096

# (6 arguments * 8 bytes)
interruptargs: .space (6 * 8)
