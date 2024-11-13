.align 2

.section .text
.globl _start
.globl stack_top

_start:
	la sp, stack_top
	li t0, 0x13
	li t1, 0x23
	sub t2, t1, t0

.section .data
