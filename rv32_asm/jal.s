.align 2

.section .text
.globl _start

_start:
	jal ra, function

function:
	li t0, 0x12345
	ret

.section .data
