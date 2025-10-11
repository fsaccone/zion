.section .text
.global disableinterrupts
.global enableinterrupts
.global interruptisuser
.global interruptsenabled
.global interrupttype
.global kernelinterrupt
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
	# 0x04 - Page fault.

	csrr t0, scause

	# Interrupt bit (I).
	li t1, 1 << 63

	# If I != 0, handle cases with interrupt bit set to 1.
	and  t2, t0, t1
	bnez t2, 1f

	# 8 | 9 -> Syscall.
	# 12 | 13 | 15 -> Page fault.
	# *     -> Exception.

	# If 8 jump to 2f.
	li  t2, 8
	beq t0, t2, 2f

	# If 9 jump to 2f.
	li  t2, 9
	beq t0, t2, 2f

	# If 12 jump to 3f.
	li  t2, 12
	beq t0, t2, 3f

	# If 13 jump to 3f.
	li  t2, 13
	beq t0, t2, 3f

	# If 15 jump to 3f.
	li  t2, 15
	beq t0, t2, 3f

	# Exception (not 8 or 9).
	li a0, 0x00
	ret

2:
	# Syscall (8 or 9).
	li a0, 0x01
	ret

3:
	# Page fault (12, 13 or 15).
	li a0, 0x04
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

	# If 5 jump to 2f.
	li  t2, 5
	beq t0, t2, 2f

	# If 7 jump to 2f.
	li  t2, 7
	beq t0, t2, 2f

	# If 9 jump to 3f.
	li  t2, 9
	beq t0, t2, 3f

	# If 11 jump to 3f.
	li  t2, 11
	beq t0, t2, 3f

	# Default (0x00 - Exception).
	li a0, 0x00
	ret

2:
	# Timer.
	li a0, 0x03
	ret

3:
	# Hardware.
	li a0, 0x02
	ret

kernelinterrupt:
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

	# Cases of cause codes.
	csrr t0, scause

	# System call.
	li   t1, 8
	beq  t0, t1, 2f
	li   t1, 9
	beq  t0, t1, 2f
	li   t1, 11
	beq  t0, t1, 2f

	# Page fault.
	li   t1, 12
	beq  t0, t1, 3f
	li   t1, 13
	beq  t0, t1, 3f
	li   t1, 15
	beq  t0, t1, 3f

	# Exception (all cases where I bit is 0 and it is not an ecall or page
	# fault).
	li   t1, 1 << 63
	and  t1, t0, t1
	beqz t1, 4f

	# Hardware.
	li  t1, 1 << 63
	li  t2, 9
	or  t2, t1, t2
	beq t0, t2, 5f
	li  t2, 11
	or  t2, t1, t2
	beq t0, t2, 5f

	# Timer.
	li  t1, 1 << 63
	li  t2, 5
	or  t2, t1, t2
	beq t0, t2, 6f
	li  t2, 7
	or  t2, t1, t2
	beq t0, t2, 6f

	# If the code was not recognized, just return from the interrupt.
	j 1f

2:
	# If cause is ecall.

	# Set sepc to the instruction after ecall.
	csrr t0,   sepc
	addi t0,   t0, 4
	csrw sepc, t0

	# The a0 register already contains the system call code.
	# Setup args array and set a1 to its address.
	la t0, syscallargs
	sd a1, 0(t0)
	sd a2, 8(t0)
	sd a3, 16(t0)
	sd a4, 24(t0)
	mv a1, t0

	# Call system call handler.
	call syscall

	j 1f

3:
	# If cause is page fault.

	call pagefault

	j 1f


4:
	# If cause is exception.

	call exception

	j 1f

5:
	# If cause is hardware.

	call exception

	j 1f

6:
	# If cause is timer.

	call timer

	j 1f

1:
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

	sret

waitforinterrupt:
	wfi

.section .data

# ((5 interrupt arguments + 1 NULL) * 8 bytes)
args: .space ((5 + 1) * 8)

# ((1 type argument + 4 arguments + 1 NULL) * 8 bytes)
syscallargs: .space ((1 + 4 + 1) * 8)
