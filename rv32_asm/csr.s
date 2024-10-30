.align 2

.section .text
.globl _start
_start:
li t0, 0x12345678
csrw mscratch, t0
csrr t1, mscratch
csrw mepc, t0
csrr t1, mepc
csrw mtval, t0
csrr t1, mtval

