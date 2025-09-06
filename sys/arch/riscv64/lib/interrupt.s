.section .text
.global disableinterrupts
.global enableinterrupts
.global interruptsenabled
.global setinterrupthandler

trapvector:
	# If interrupthandler is not yet set, just sret
	la   t0, interrupthandler
	beqz t0, 1f

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

	ld   t0, interrupthandler
	jalr ra, t0, 0

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

1:
	sret

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

	ret

interruptsenabled:
	csrr a0, sstatus
	li   t0, 1 << 1
	and  a0, a0, t0
	srli a0, a0, 1

	ret

setinterrupthandler:
	la t0, interrupthandler
	sd a0, 0(t0)

	la   t0,    trapvector
	csrw stvec, t0

	ret

.section .data

# Pointer to the interrupt handler function
interrupthandler: .dword 0
