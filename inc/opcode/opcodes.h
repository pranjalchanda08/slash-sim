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

