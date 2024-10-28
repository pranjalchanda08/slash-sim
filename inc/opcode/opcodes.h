#pragma once

typedef enum opcode
{
    RV32_OPCODE_LOAD = 0x03, // 5 different Load insructions
    RV32_OPCODE_ALUI = 0x13, // 9 different ALU immidiate insructions
    RV32_OPCODE_AUIPC = 0x17,
    RV32_OPCODE_STORE = 0x23, // 3 different Store instructions
    RV32_OPCODE_ALUR = 0x33,  // 10 different ALU reg instructions
    RV32_OPCODE_LUI = 0x37,
    RV32_OPCODE_BRANCH = 0x63, // 6 different branch instructions
    RV32_OPCODE_JALR = 0x67,
    RV32_OPCODE_JAL = 0x6F,
    RV32_OPCODE_CSR = 0x73, // 8 different CSR instructions
} rv32_opcode_t;


// Machine CSRs
#define CSR_MSTATUS          0x300  // Machine Status (lower 32 bits)
#define CSR_MISA             0x301  // Machine ISA
#define CSR_MIE              0x304  // Machine Interrupt Enable Register
#define CSR_MTVEC            0x305  // Machine Trap-Handler Base Address
#define CSR_MTVT             0x307  // Machine Trap-Handler Vector Table Base Address
#define CSR_MSTATUSH         0x310  // Machine Status (upper 32 bits)
#define CSR_MCOUNTINHIBIT    0x320  // Machine Counter-Inhibit Register
#define CSR_MHPMEVENT3       0x323  // Machine Performance-Monitoring Event Selector 3
#define CSR_MHPMEVENT31      0x33F  // Machine Performance-Monitoring Event Selector 31
#define CSR_MSCRATCH         0x340  // Machine Scratch
#define CSR_MEPC             0x341  // Machine Exception Program Counter
#define CSR_MCAUSE           0x342  // Machine Trap Cause
#define CSR_MTVAL            0x343  // Machine Trap Value
#define CSR_MIP              0x344  // Machine Interrupt Pending Register
#define CSR_MNXTI            0x345  // Interrupt handler address and enable modifier
#define CSR_MINTTHRESH       0x347  // Interrupt-level threshold
#define CSR_MSCRATCHCSWL     0x349  // Conditional scratch swap on level change
#define CSR_TSELECT          0x7A0  // Trigger Select Register
#define CSR_TDATA1           0x7A1  // Trigger Data Register 1
#define CSR_TDATA2           0x7A2  // Trigger Data Register 2
#define CSR_TINFO            0x7A4  // Trigger Info
#define CSR_DCSR             0x7B0  // Debug Control and Status
#define CSR_DPC              0x7B1  // Debug PC
#define CSR_DSATCH0          0x7B2  // Debug Scratch Register 0
#define CSR_DSATCH1          0x7B3  // Debug Scratch Register 1
#define CSR_MCYCLE           0xB00  // Machine Cycle Counter
#define CSR_MINSTRET         0xB02  // Machine Instructions-Retired Counter
#define CSR_MHPMCOUNTER3     0xB03  // Machine Performance-Monitoring Counter 3
#define CSR_MHPMCOUNTER31    0xB1F  // Machine Performance-Monitoring Counter 31
#define CSR_MCYCLEH          0xB80  // Upper 32 Machine Cycle Counter
#define CSR_MINSTRETH        0xB82  // Upper 32 Machine Instructions-Retired Counter
#define CSR_MHPMCOUNTERH3    0xB83  // Upper 32 Machine Performance-Monitoring Counter 3
#define CSR_MHPMCOUNTERH31   0xB9F  // Upper 32 Machine Performance-Monitoring Counter 31
#define CSR_MVENDORID        0xF11  // Machine Vendor ID
#define CSR_MARCHID          0xF12  // Machine Architecture ID
#define CSR_MIMPID           0xF13  // Machine Implementation ID
#define CSR_MHARTID          0xF14  // Hardware Thread ID
#define CSR_MCONFIGPTR       0xF15  // Machine Configuration Pointer
#define CSR_MINTSTATUS       0xFB1  // Current interrupt levels

// Additional custom CSRs
#define CSR_CPUCTRL          0xBF0  // CPU control
#define CSR_SECURESEED0      0xBF9  // Seed for LFSR0
#define CSR_SECURESEED1      0xBFA  // Seed for LFSR1
#define CSR_SECURESEED2      0xBFC  // Seed for LFSR2

// Unprivileged and User-Level CSRs
#define CSR_JVT              0x017  // Table jump base vector and control register

// Machine CSRs for environment configuration
#define CSR_MCOUNTEREN       0x306  // Machine Counter Enable
#define CSR_MENVCFG          0x30A  // Machine Environment Configuration (lower 32 bits)
#define CSR_MSTATEEN0        0x30C  // Machine state enable 0 (lower 32 bits)
#define CSR_MSTATEEN1        0x30D  // Machine state enable 1 (lower 32 bits)
#define CSR_MSTATEEN2        0x30E  // Machine state enable 2 (lower 32 bits)
#define CSR_MSTATEEN3        0x30F  // Machine state enable 3 (lower 32 bits)
#define CSR_MENVCFGH         0x31A  // Machine Environment Configuration (upper 32 bits)
#define CSR_MSTATEEN0H       0x31C  // Machine state enable 0 (upper 32 bits)
#define CSR_MSTATEEN1H       0x31D  // Machine state enable 1 (upper 32 bits)
#define CSR_MSTATEEN2H       0x31E  // Machine state enable 2 (upper 32 bits)
#define CSR_MSTATEEN3H       0x31F  // Machine state enable 3 (upper 32 bits)
#define CSR_MSCRATCHCSW      0x348  // Conditional scratch swap on priv mode change

// Physical memory protection configuration
#define CSR_PMPCFG0          0x3A0  // Physical memory protection configuration.
#define CSR_PMPCFG1          0x3A1  // Physical memory protection configuration.
#define CSR_PMPCFG2          0x3A2  // Physical memory protection configuration.
#define CSR_PMPCFG15         0x3AF  // Physical memory protection configuration.
#define CSR_PMPADDR0         0x3B0  // Physical memory protection address register.
#define CSR_PMPADDR1         0x3B1  // Physical memory protection address register.
#define CSR_PMPADDR2         0x3B2  // Physical memory protection address register.
#define CSR_PMPADDR63        0x3EF  // Physical memory protection address register.
#define CSR_MSECCFG          0x747  // Machine Security Configuration (lower 32 bits).
#define CSR_MSECCFGH         0x757  // Machine Security Configuration (upper 32 bits).
