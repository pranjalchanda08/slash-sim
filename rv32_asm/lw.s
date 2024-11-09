.align 2

.section .text
.globl _start

_start: 
	la a0, data_val
	lhu a1, 0(a0)

	la a2, data_val
	lw a3, 0(a2)

.section .data
data_val:
	.word 0x123
