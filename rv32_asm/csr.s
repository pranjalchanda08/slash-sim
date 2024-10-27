.align 2

.section .text
.globl _start
_start:
    csrr a3, mcause   
    csrw mstatus, t6
    csrr a1 ,misa 
    csrr t1, mip
    csrr a2, mtval 
    csrr t6, mcycle
    csrw mcycle, t6