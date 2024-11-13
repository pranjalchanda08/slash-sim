.align 2

.section .text
.globl _start
_start:
    li t0, 0x12345678
    csrw mscratch, t0   # Write the value of register t0 into the machine scratch register (mscratch)
    csrr t1, mscratch   # Read the value from the machine scratch register (mscratch) into register t1
    csrw mepc, t0       # Write the value of register t0 into the machine exception program counter (mepc)
    csrr t1, mepc       # Read the value from the machine exception program counter (mepc) into register t1
    csrw mtval, t0      # Write the value of register t0 into the machine trap value register (mtval)
    csrr t1, mtval      # Read the value from the machine trap value register (mtval) into register t1

