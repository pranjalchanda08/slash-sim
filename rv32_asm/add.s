.align 2

.section .text
.globl _start
.globl stack_top

_start:
    la sp, stack_top
    li t0, 0x13
    li t1, 0x23
    csrr a0, mcause   
    csrw mstatus, t6
    csrr a1 ,misa 
    add t2, t0, t1
    li       t1, 0x80               
    la t4, msg
    
.section .data
msg:
    .string "Pranjal\n"
    .string "Chanda\n"