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

	la   t0,    handleinterrupt
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
