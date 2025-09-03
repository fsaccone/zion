.section .text
.global disableinterrupts
.global enableinterrupts
.global interruptsenabled

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
