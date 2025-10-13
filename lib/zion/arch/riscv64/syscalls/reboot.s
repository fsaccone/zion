.section .text
.global reboot

reboot:
	li a0, 2
	ecall

	ret
