/*****************************************************************************************
 * SLASH-SIM LICENSE
 * Copyrights (C) <2024>, Pranjal Chanda
 *
 * @file    rv32_csr.h
 * @brief   Declarations related to CSR registers
 *****************************************************************************************/
#pragma once

// Machine RV32_CSRs
#define RV32_CSR_MSTATUS          0x300  // Machine Status (lower 32 bits)
#define RV32_CSR_MISA             0x301  // Machine ISA
#define RV32_CSR_MIE              0x304  // Machine Interrupt Enable Register
#define RV32_CSR_MTVEC            0x305  // Machine Trap-Handler Base Address
#define RV32_CSR_MTVT             0x307  // Machine Trap-Handler Vector Table Base Address
#define RV32_CSR_MSTATUSH         0x310  // Machine Status (upper 32 bits)
#define RV32_CSR_MCOUNTINHIBIT    0x320  // Machine Counter-Inhibit Register
#define RV32_CSR_MHPMEVENT3       0x323  // Machine Performance-Monitoring Event Selector 3
#define RV32_CSR_MHPMEVENT31      0x33F  // Machine Performance-Monitoring Event Selector 31
#define RV32_CSR_MSCRATCH         0x340  // Machine Scratch
#define RV32_CSR_MEPC             0x341  // Machine Exception Program Counter
#define RV32_CSR_MCAUSE           0x342  // Machine Trap Cause
#define RV32_CSR_MTVAL            0x343  // Machine Trap Value
#define RV32_CSR_MIP              0x344  // Machine Interrupt Pending Register
#define RV32_CSR_MNXTI            0x345  // Interrupt handler address and enable modifier
#define RV32_CSR_MINTTHRESH       0x347  // Interrupt-level threshold
#define RV32_CSR_MSCRATCHCSWL     0x349  // Conditional scratch swap on level change
#define RV32_CSR_TSELECT          0x7A0  // Trigger Select Register
#define RV32_CSR_TDATA1           0x7A1  // Trigger Data Register 1
#define RV32_CSR_TDATA2           0x7A2  // Trigger Data Register 2
#define RV32_CSR_TINFO            0x7A4  // Trigger Info
#define RV32_CSR_DCSR             0x7B0  // Debug Control and Status
#define RV32_CSR_DPC              0x7B1  // Debug PC
#define RV32_CSR_DSATCH0          0x7B2  // Debug Scratch Register 0
#define RV32_CSR_DSATCH1          0x7B3  // Debug Scratch Register 1
#define RV32_CSR_MCYCLE           0xB00  // Machine Cycle Counter
#define RV32_CSR_MINSTRET         0xB02  // Machine Instructions-Retired Counter
#define RV32_CSR_MHPMCOUNTER3     0xB03  // Machine Performance-Monitoring Counter 3
#define RV32_CSR_MHPMCOUNTER31    0xB1F  // Machine Performance-Monitoring Counter 31
#define RV32_CSR_MCYCLEH          0xB80  // Upper 32 Machine Cycle Counter
#define RV32_CSR_MINSTRETH        0xB82  // Upper 32 Machine Instructions-Retired Counter
#define RV32_CSR_MHPMCOUNTERH3    0xB83  // Upper 32 Machine Performance-Monitoring Counter 3
#define RV32_CSR_MHPMCOUNTERH31   0xB9F  // Upper 32 Machine Performance-Monitoring Counter 31
#define RV32_CSR_MVENDORID        0xF11  // Machine Vendor ID
#define RV32_CSR_MARCHID          0xF12  // Machine Architecture ID
#define RV32_CSR_MIMPID           0xF13  // Machine Implementation ID
#define RV32_CSR_MHARTID          0xF14  // Hardware Thread ID
#define RV32_CSR_MCONFIGPTR       0xF15  // Machine Configuration Pointer
#define RV32_CSR_MINTSTATUS       0xFB1  // Current interrupt levels

// Additional custom RV32_CSRs
#define RV32_CSR_CPUCTRL          0xBF0  // CPU control
#define RV32_CSR_SECURESEED0      0xBF9  // Seed for LFSR0
#define RV32_CSR_SECURESEED1      0xBFA  // Seed for LFSR1
#define RV32_CSR_SECURESEED2      0xBFC  // Seed for LFSR2

// Unprivileged and User-Level RV32_CSRs
#define RV32_CSR_JVT              0x017  // Table jump base vector and control register

// Machine RV32_CSRs for environment configuration
#define RV32_CSR_MCOUNTEREN       0x306  // Machine Counter Enable
#define RV32_CSR_MENVCFG          0x30A  // Machine Environment Configuration (lower 32 bits)
#define RV32_CSR_MSTATEEN0        0x30C  // Machine state enable 0 (lower 32 bits)
#define RV32_CSR_MSTATEEN1        0x30D  // Machine state enable 1 (lower 32 bits)
#define RV32_CSR_MSTATEEN2        0x30E  // Machine state enable 2 (lower 32 bits)
#define RV32_CSR_MSTATEEN3        0x30F  // Machine state enable 3 (lower 32 bits)
#define RV32_CSR_MENVCFGH         0x31A  // Machine Environment Configuration (upper 32 bits)
#define RV32_CSR_MSTATEEN0H       0x31C  // Machine state enable 0 (upper 32 bits)
#define RV32_CSR_MSTATEEN1H       0x31D  // Machine state enable 1 (upper 32 bits)
#define RV32_CSR_MSTATEEN2H       0x31E  // Machine state enable 2 (upper 32 bits)
#define RV32_CSR_MSTATEEN3H       0x31F  // Machine state enable 3 (upper 32 bits)
#define RV32_CSR_MSCRATCHCSW      0x348  // Conditional scratch swap on priv mode change

// Physical memory protection configuration
#define RV32_CSR_PMPCFG0          0x3A0  // Physical memory protection configuration.
#define RV32_CSR_PMPCFG1          0x3A1  // Physical memory protection configuration.
#define RV32_CSR_PMPCFG2          0x3A2  // Physical memory protection configuration.
#define RV32_CSR_PMPCFG15         0x3AF  // Physical memory protection configuration.
#define RV32_CSR_PMPADDR0         0x3B0  // Physical memory protection address register.
#define RV32_CSR_PMPADDR1         0x3B1  // Physical memory protection address register.
#define RV32_CSR_PMPADDR2         0x3B2  // Physical memory protection address register.
#define RV32_CSR_PMPADDR63        0x3EF  // Physical memory protection address register.
#define RV32_CSR_MSECCFG          0x747  // Machine Security Configuration (lower 32 bits).
#define RV32_CSR_MSECCFGH         0x757  // Machine Security Configuration (upper 32 bits).

typedef struct _rv32i_csr
{
    uint32_t mstatus;        // Machine Status Register
    uint32_t misa;           // Machine ISA Register
    uint32_t mie;            // Machine Interrupt Enable Register
    uint32_t mtvec;          // Machine Trap-Vector Base Address
    uint32_t mtvt;           // Machine Trap-Vector Table (if implemented)
    uint32_t mstatush;       // Machine Status Register (high)
    uint32_t mcountinhibit;  // Machine Counter Inhibit
    uint32_t mhpmevent3;     // Machine Performance Monitor Event 3
    uint32_t mhpmevent31;    // Machine Performance Monitor Event 31
    uint32_t mscratch;       // Machine Scratch Register
    uint32_t mepc;           // Machine Exception Program Counter
    uint32_t mcause;         // Machine Cause Register
    uint32_t mtval;          // Machine Trap Value Register
    uint32_t mip;            // Machine Interrupt Pending Register
    uint32_t mnxti;          // Machine Next Interrupt (if implemented)
    uint32_t mintthresh;     // Machine Interrupt Threshold (if implemented)
    uint32_t mscratchcswl;   // Machine Scratch Swap Lower (if implemented)
    uint32_t tselect;        // Debug/Trace Trigger Select
    uint32_t tdata1;         // Debug/Trace Data 1
    uint32_t tdata2;         // Debug/Trace Data 2
    uint32_t tinfo;          // Debug/Trace Info
    uint32_t dcsr;           // Debug Control and Status Register
    uint32_t dpc;            // Debug Program Counter
    uint32_t dscratch0;      // Debug Scratch Register 0
    uint32_t dscratch1;      // Debug Scratch Register 1
    uint32_t mcycle;         // Machine Cycle Counter
    uint32_t minstret;       // Machine Instructions Retired Counter

} rv32i_csr_t;

typedef struct {
    uint32_t address;
    uint32_t *value;
} csr_t;
