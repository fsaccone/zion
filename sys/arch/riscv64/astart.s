.section .text
.global astart

astart:
	la sp, stackend

	call initmisa
	call delegate
	call lockharts

	call initmstatus
	call initsstatus

	la   t0,    trapvec
	csrw stvec, t0

	j supervisor

delegate:
	li   t0,      0xffff
	csrw medeleg, t0
	csrw mideleg, t0

	# Set sie.STIE and sie.SEIE both to 1
	csrr t0,  sie
	li   t1,  (1 << 5) | (1 << 9)
	or   t0,  t0, t1
	csrw sie, t0

	ret

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

	# Enable the sstc extension
	csrr t0,      menvcfg
	li   t1,      (1 << 63)
	or   t0,      t0, t1
	csrw menvcfg, t0

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

trapvec:
	# ((7 t-regs + 12 s-regs + 8 a-regs + 1 ra + 1 gp) * 8 bytes)
	addi sp,  sp, -((7 + 12 + 8 + 1 + 1) * 8)
	sd   t0,  (0 * 8 )(sp)
	sd   t1,  (1 * 8 )(sp)
	sd   t2,  (2 * 8 )(sp)
	sd   t3,  (3 * 8 )(sp)
	sd   t4,  (4 * 8 )(sp)
	sd   t5,  (5 * 8 )(sp)
	sd   t6,  (6 * 8 )(sp)
	sd   s0,  (7 * 8 )(sp)
	sd   s1,  (8 * 8 )(sp)
	sd   s2,  (9 * 8 )(sp)
	sd   s3,  (10 * 8)(sp)
	sd   s4,  (11 * 8)(sp)
	sd   s5,  (12 * 8)(sp)
	sd   s6,  (13 * 8)(sp)
	sd   s7,  (14 * 8)(sp)
	sd   s8,  (15 * 8)(sp)
	sd   s9,  (16 * 8)(sp)
	sd   s10, (17 * 8)(sp)
	sd   s11, (18 * 8)(sp)
	sd   a0,  (19 * 8)(sp)
	sd   a1,  (20 * 8)(sp)
	sd   a2,  (21 * 8)(sp)
	sd   a3,  (22 * 8)(sp)
	sd   a4,  (23 * 8)(sp)
	sd   a5,  (24 * 8)(sp)
	sd   a6,  (25 * 8)(sp)
	sd   a7,  (26 * 8)(sp)
	sd   ra,  (27 * 8)(sp)
	sd   gp,  (28 * 8)(sp)

	call interrupt

	# See top of function
	ld   t0,  (0 * 8 )(sp)
	ld   t1,  (1 * 8 )(sp)
	ld   t2,  (2 * 8 )(sp)
	ld   t3,  (3 * 8 )(sp)
	ld   t4,  (4 * 8 )(sp)
	ld   t5,  (5 * 8 )(sp)
	ld   t6,  (6 * 8 )(sp)
	ld   s0,  (7 * 8 )(sp)
	ld   s1,  (8 * 8 )(sp)
	ld   s2,  (9 * 8 )(sp)
	ld   s3,  (10 * 8)(sp)
	ld   s4,  (11 * 8)(sp)
	ld   s5,  (12 * 8)(sp)
	ld   s6,  (13 * 8)(sp)
	ld   s7,  (14 * 8)(sp)
	ld   s8,  (15 * 8)(sp)
	ld   s9,  (16 * 8)(sp)
	ld   s10, (17 * 8)(sp)
	ld   s11, (18 * 8)(sp)
	ld   a0,  (19 * 8)(sp)
	ld   a1,  (20 * 8)(sp)
	ld   a2,  (21 * 8)(sp)
	ld   a3,  (22 * 8)(sp)
	ld   a4,  (23 * 8)(sp)
	ld   a5,  (24 * 8)(sp)
	ld   a6,  (25 * 8)(sp)
	ld   a7,  (26 * 8)(sp)
	ld   ra,  (27 * 8)(sp)
	ld   gp,  (28 * 8)(sp)
	addi sp,  sp, ((7 + 12 + 8 + 1 + 1) * 8)

	sret

supervisor:
	la   t0,   callkernel
	csrw mepc, t0

	# Allow supervisor to use stimecmp and time
	csrr t0,         mcounteren
	li   t1,         (1 << 1)
	or   t0,         t0, t1
	csrw mcounteren, t0

	# Kernel PMP address range (all addresses)
	li   t0,       0x0
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

callkernel:
	j kernel

.section .data

stackend: .space 4096
