.section .text
.global disableinterrupts
.global enableinterrupts
.global interruptsenabled
.global kernelinterrupt
.global userinterrupt

# Checks the value of scause to call the correct handler.
routeinterrupt:
	addi sp, sp, -8
	sd   ra, 0(sp)

	# Save scause to s0.
	csrr s0, scause

	# System call.
	li   t0, 8
	beq  s0, t0, 2f

	# Page fault.
	li   t0, 12
	beq  s0, t0, 3f
	li   t0, 13
	beq  s0, t0, 3f
	li   t0, 15
	beq  s0, t0, 3f

	# Exception (all cases where I bit is 0 and it is not an user ecall or
	# page fault).
	li   t0, 1 << 63
	and  t0, s0, t0
	beqz t0, 4f

	# Hardware.
	li  t0, 1 << 63
	li  t1, 9
	or  t1, t0, t1
	beq s0, t1, 5f
	li  t1, 11
	or  t1, t0, t1
	beq s0, t1, 5f

	# Timer.
	li  t0, 1 << 63
	li  t1, 5
	or  t1, t0, t1
	beq s0, t1, 6f
	li  t1, 7
	or  t1, t0, t1
	beq s0, t1, 6f

	# If the code was not recognized, just return.
	j 1f

2:
	# If cause is ecall.

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

	csrr a0, sstatus
	srli a0, a0, 8
	andi a0, a0, 1
	xori a0, a0, 1

	call pagefault

	j 1f


4:
	# If cause is exception.

	csrr a0, sstatus
	srli a0, a0, 8
	andi a0, a0, 1
	xori a0, a0, 1

	call exception

	j 1f

5:
	# If cause is hardware.

	call hardware

	j 1f

6:
	# If cause is timer.

	call timer

	j 1f

1:
	ld   ra, 0(sp)
	addi sp, sp, 8
	ret

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

interruptsenabled:
	csrr a0, sstatus
	li   t0, 1 << 1
	and  a0, a0, t0
	srli a0, a0, 1

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

	call routeinterrupt

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

userinterrupt:
	addi sp, sp, -8
	sd   ra, 0(sp)

	call routeinterrupt

	# Do not do sret, since this function is only called as part of
	# trampoline.
	ld   ra, 0(sp)
	addi sp, sp, 8
	ret

.section .data

# ((1 type argument + 4 arguments + 1 NULL) * 8 bytes)
syscallargs: .space ((1 + 4 + 1) * 8)
