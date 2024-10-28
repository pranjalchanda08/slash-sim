.align 2

.section .text
.globl _start

_start: 
	la t0, function
	jalr ra, 0(t0)

function:
	li t1, 0x12345
	ret

.section .data
