.section .text
.global astart

astart:
	# Switch to physical memory.
	csrwi satp, 0

	call initmisa
	call delegate

	call initpmp
	call initmstatus
	call initsstatus
	call initstime

	la   t0,    trapvec
	csrw stvec, t0

	csrr tp, mhartid

	# (sp = kernelstacks[(tp + 1) * 4096])
	la   sp, kernelstacks
	mv   t0, tp
	addi a0, t0, 1
	li   a1, 4096
	call multiply
	add  sp, sp, a0

	la   t0,   kernel
	csrw mepc, t0

	# Set mstatus.MPP[0] to 1 and mstatus.MPP[1] to 0 (mstatus.MPP = 0b01):
	# this makes mret switch to supervisor mode.
	csrr t0, mstatus
	li   t1,      1 << 11
	or   t0,      t0, t1
	li   t1,      1 << 12
	not  t1,      t1
	and  t0,      t0, t1
	csrw mstatus, t0

	mret

delegate:
	li   t0,      0xffff
	csrw medeleg, t0
	csrw mideleg, t0

	# Set mie.STIE to 1.
	csrr t0,  mie
	li   t1,  (1 << 5)
	or   t0,  t0, t1
	csrw mie, t0

	# Set sie.STIE and sie.SEIE both to 1.
	csrr t0,  sie
	li   t1,  (1 << 5) | (1 << 9)
	or   t0,  t0, t1
	csrw sie, t0

	ret

initmisa:
	li   t0, 0

	# RV32I/64I base ISA.
	li  t1, 1 << 8
	add t0, t0, t1

	# Supervisor mode implemented.
	li  t1, 1 << 18
	add t0, t0, t1

	# User mode implemented.
	li  t1, 1 << 20
	add t0, t0, t1

	csrw misa, t0

	ret

initmstatus:
	csrr t0, mstatus

	# Set SIE to 1.
	li t1, 1 << 1
	or t0, t0, t1

	# Set MIE to 1.
	li t1, 1 << 3
	or t0, t0, t1

	# Set MPIE to 0.
	li  t1, 1 << 7
	not t1, t1
	and t0, t0, t1

	csrw mstatus, t0

	ret

initsstatus:
	csrr t0, sstatus

	# Set SUM to 1.
	li t1, 1 << 18
	or t0, t0, t1

	# Set MXR to 1.
	li t1, 1 << 19
	or t0, t0, t1

	csrw sstatus, t0

	ret

initpmp:
	# Kernel PMP address range (all addresses).
	li   t0,       0x0
	li   t1,       0xffffffffffffffff
	csrw pmpaddr0, t0
	csrw pmpaddr1, t1

	# Kernel PMP configuration.
	li   t0,      0
	li   t1,      1 << 0 # Read permission.
	or   t0,      t0, t1
	li   t1,      1 << 1 # Write permission.
	or   t0,      t0, t1
	li   t1,      1 << 2 # Execute permission.
	or   t0,      t0, t1
	li   t1,      1 << 3 # TOR address-matching mode.
	or   t0,      t0, t1
	li   t1,      1 << 7 # Lock.
	or   t0,      t0, t1
	slli t0,      t0, 8  # Write to pmp1cfg.
	csrw pmpcfg0, t0

	ret

initstime:
	# Enable the sstc extension.
	csrr t0,      menvcfg
	li   t1,      (1 << 63)
	or   t0,      t0, t1
	csrw menvcfg, t0

	# Allow supervisor to use stimecmp and time.
	csrr t0,         mcounteren
	li   t1,         (1 << 1)
	or   t0,         t0, t1
	csrw mcounteren, t0

	ret

# (a0 = a0 * a1)
# Only works with unsigned values.
multiply:
	li t0, 0

1:
	# If (a1 <= 0) then return.
	blez a1, 2f

	# (t0 += a0)
	add t0, t0, a0

	# (a1--)
	addi a1, a1, -1

	# Loop back.
	j 1b

2:
	mv a0, t0
	ret

trapvec:
	# Switch to physical memory
	csrwi satp, 0

	# ((7 t-regs + 12 s-regs + 7 a-regs + 1 ra + 1 gp) * 8 bytes)
	addi sp,  sp, -((7 + 12 + 7 + 1 + 1) * 8)
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
	sd   a1,  (19 * 8)(sp)
	sd   a2,  (20 * 8)(sp)
	sd   a3,  (21 * 8)(sp)
	sd   a4,  (22 * 8)(sp)
	sd   a5,  (23 * 8)(sp)
	sd   a6,  (24 * 8)(sp)
	sd   a7,  (25 * 8)(sp)
	sd   ra,  (26 * 8)(sp)
	sd   gp,  (27 * 8)(sp)

	# The a0 register is held by sscratch instead of the stack so that we
	# can choose to change its value when switching back to the caller
	# context.
	csrw sscratch, a0

	# If type is 8, 9 or 11 then the trap is an ecall exception: sepc needs
	# to increment by 4, the size of the ecall instruction, before
	# returning to avoid looping back to the same ecall instruction
	# address.
	csrr t0, scause
	li   t1, 8
	beq  t0, t1, 1f
	li   t1, 9
	beq  t0, t1, 1f
	li   t1, 11
	beq  t0, t1, 1f
	j    2f

1:
	# Only reached if cause is ecall.
	csrr t0,   sepc
	addi t0,   t0, 4
	csrw sepc, t0

2:
	call interrupt

	# See top of function.
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
	ld   a1,  (19 * 8)(sp)
	ld   a2,  (20 * 8)(sp)
	ld   a3,  (21 * 8)(sp)
	ld   a4,  (22 * 8)(sp)
	ld   a5,  (23 * 8)(sp)
	ld   a6,  (24 * 8)(sp)
	ld   a7,  (25 * 8)(sp)
	ld   ra,  (26 * 8)(sp)
	ld   gp,  (27 * 8)(sp)
	addi sp,  sp, ((7 + 12 + 7 + 1 + 1) * 8)

	# The sscratch register may hold the original value of a0 or a new one.
	# See above.
	csrr a0, sscratch

	sret
