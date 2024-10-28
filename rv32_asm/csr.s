.align 2

.section .text
.globl _start
_start:
    csrr a3, mcause
    li t6 ,0x42
    csrw mstatus, t6
    csrr a3 , mstatus
    csrr a1 ,misa
    csrr a2, mtval
    csrr t6, mcycle
    csrw mcycle, t6
