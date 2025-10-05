.section .text
.global disableinterrupts
.global enableinterrupts
.global interruptargs
.global interruptisuser
.global interruptsenabled
.global interrupttype
.global setinterruptreturn
.global waitforinterrupt

disableinterrupts:
	csrr t0,      sstatus
	li   t1,      1 << 1
	not  t1,      t1
	and  t0,      t0, t1
	csrw sstatus, t0

	ret

enableinterrupts:
	csrr t0,      sstatus
	li   t1,      1 << 1
	or   t0,      t0, t1
	csrw sstatus, t0

	# Set sie.SSIE, sie.STIE and sie.SEIE to 1.
	li   t0, 0
	li   t1,  1 << 1
	or   t0,  t0, t1
	li   t1,  1 << 5
	or   t0,  t0, t1
	li   t1,  1 << 9
	or   t0,  t0, t1
	csrw sie, t0

	ret

interruptargs:
	la t0, args
	li t1, 0

	sd a0, (0 * 8)(t0)
	sd a1, (1 * 8)(t0)
	sd a2, (2 * 8)(t0)
	sd a3, (3 * 8)(t0)
	sd a4, (4 * 8)(t0)
	sd t1, (5 * 8)(t0)

	mv a0, t0

	ret

interruptisuser:
	# (t0 = sstatus.SPP)
	csrr t0, sstatus
	srli t0, t0, 8
	li   t1, 1
	and  t0, t0, t1

	# If t0 is 0, then interrupt was caused by user mode; otherwise, it was
	# caused by supervisor mode.
	beqz t0, 1f

	# Kernel mode.
	li a0, 0
	ret

1:
	# User mode.
	li a0, 1
	ret

interruptsenabled:
	csrr a0, sstatus
	li   t0, 1 << 1
	and  a0, a0, t0
	srli a0, a0, 1

	ret

interrupttype:
	# 0x00 - Exception.
	# 0x01 - Syscall.
	# 0x02 - Hardware.
	# 0x03 - Timer.

	csrr t0, scause

	# Interrupt bit (I).
	li t1, 1 << 63

	# If I != 0, handle cases with interrupt bit set to 1.
	and  t2, t0, t1
	bnez t2, 1f

	# 8 | 9 -> Syscall.
	# *     -> Exception.

	# If 8 jump to 2f.
	li  t2, 8
	beq t0, t2, 2f

	# If 9 jump to 2f.
	li  t2, 9
	beq t0, t2, 2f

	# Exception (not 8).
	li a0, 0x00
	ret

2:
	# Syscall (8).
	li a0, 0x01
	ret

1:
	# Set I to 0.
	not t2, t1
	and t0, t0, t2

	# 1 || 3  -> Syscall.
	# 5 || 7  -> Timer.
	# 9 || 11 -> Hardware.

	# If 1 jump to 2b.
	li  t2, 1
	beq t0, t2, 2b

	# If 3 jump to 2b.
	li  t2, 3
	beq t0, t2, 2b

	# If 5 jump to 3f.
	li  t2, 5
	beq t0, t2, 3f

	# If 7 jump to 3f.
	li  t2, 7
	beq t0, t2, 3f

	# If 9 jump to 4f.
	li  t2, 9
	beq t0, t2, 4f

	# If 11 jump to 4f.
	li  t2, 11
	beq t0, t2, 4f

	# Default (0x00 - Exception).
	li a0, 0x00
	ret

3:
	# Timer.
	li a0, 0x03
	ret

4:
	# Hardware.
	li a0, 0x02
	ret

setinterruptreturn:
	# The sscratch is used to hold the new value of a0 and is set to the
	# original a0 at the start of the trap vector.
	csrw sscratch, a0

	ret

waitforinterrupt:
	wfi

.section .data

# ((5 interrupt arguments + 1 NULL) * 8 bytes)
args: .space ((5 + 1) * 8)
