.section .text
.global reboot

reboot:
	li a0, 1
	ecall

	ret
