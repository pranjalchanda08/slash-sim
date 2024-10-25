#include "rv32i_fetch_decode.h"

rv32_opcode_sub_fn_t alui_sub_list[] = {
    RV32_SUB_FUNC_ENTRY(0x00, 0x00, RV_INS_TYPE_I1, execute_alui_i1_add,    "addi\t$rd, $rs1, $i"),
    RV32_SUB_FUNC_ENTRY(0x02, 0x00, RV_INS_TYPE_I1, execute_alui_i1_slti,   "slti\t$rd, $rs1, $i"),
    RV32_SUB_FUNC_ENTRY(0x03, 0x00, RV_INS_TYPE_I1, execute_alui_i1_sltiu,  "sltiu\t$rd, $rs1, $i"),
    RV32_SUB_FUNC_ENTRY(0x04, 0x00, RV_INS_TYPE_I1, execute_alui_i1_xori,   "xori\t$rd, $rs1, $i"),
    RV32_SUB_FUNC_ENTRY(0x06, 0x00, RV_INS_TYPE_I1, execute_alui_i1_ori,    "ori\t$rd, $rs1, $i"),
    RV32_SUB_FUNC_ENTRY(0x07, 0x00, RV_INS_TYPE_I1, execute_alui_i1_andi,   "andi\t$rd, $rs1, $i"),
    RV32_SUB_FUNC_ENTRY(0x01, 0x00, RV_INS_TYPE_I2, execute_alui_i2_slli,   "slli\t$rd, $rs1, $i"),
    RV32_SUB_FUNC_ENTRY(0x05, 0x00, RV_INS_TYPE_I2, execute_alui_i2_srli,   "srli\t$rd, $rs1, $i"),
    RV32_SUB_FUNC_ENTRY(0x05, 0x20, RV_INS_TYPE_I2, execute_alui_i2_srli,   "srai\t$rd, $rs1, $i"),
};

rv32_opcode_sub_fn_t alur_sub_list[] = {
    RV32_SUB_FUNC_ENTRY(0x00, 0x00, RV_INS_TYPE_R, execute_alur_r_add,  "add\t$rd, $rs1, $rs2"),
    RV32_SUB_FUNC_ENTRY(0x00, 0x20, RV_INS_TYPE_R, execute_alur_r_sub,  "sub\t$rd, $rs1, $rs2"),
    RV32_SUB_FUNC_ENTRY(0x01, 0x00, RV_INS_TYPE_R, execute_alur_r_sll,  "sll\t$rd, $rs1, $rs2"),
    RV32_SUB_FUNC_ENTRY(0x02, 0x00, RV_INS_TYPE_R, execute_alur_r_slt,  "slt\t$rd, $rs1, $rs2"),
    RV32_SUB_FUNC_ENTRY(0x03, 0x00, RV_INS_TYPE_R, execute_alur_r_sltu, "sltu\t$rd, $rs1, $rs2"),
    RV32_SUB_FUNC_ENTRY(0x04, 0x00, RV_INS_TYPE_R, execute_alur_r_xor,  "xor\t$rd, $rs1, $rs2"),
    RV32_SUB_FUNC_ENTRY(0x05, 0x00, RV_INS_TYPE_R, execute_alur_r_srl,  "srl\t$rd, $rs1, $rs2"),
    RV32_SUB_FUNC_ENTRY(0x05, 0x20, RV_INS_TYPE_R, execute_alur_r_sra,  "sra\t$rd, $rs1, $rs2"),
    RV32_SUB_FUNC_ENTRY(0x06, 0x00, RV_INS_TYPE_R, execute_alur_r_or,   "or\t$rd, $rs1, $rs2"),
    RV32_SUB_FUNC_ENTRY(0x07, 0x00, RV_INS_TYPE_R, execute_alur_r_and,  "and\t$rd, $rs1, $rs2"),
};

rv32_opcode_sub_fn_t store_sub_list[] = {
    RV32_SUB_FUNC_ENTRY(0x00, 0x00, RV_INS_TYPE_S, execute_store_s_sb, "sb\t$rs2, $i($rs1)"),
    RV32_SUB_FUNC_ENTRY(0x01, 0x00, RV_INS_TYPE_S, execute_store_s_sh, "sh\t$rs2, $i($rs1)"),
    RV32_SUB_FUNC_ENTRY(0x02, 0x00, RV_INS_TYPE_S, execute_store_s_sw, "sw\t$rs2, $i($rs1)"),
};

rv32_opcode_sub_fn_t csr_sub_list[] = {
    RV32_SUB_FUNC_ENTRY(0x00, 0x00, RV_INS_TYPE_I1, execute_csr, "ecall"),
    RV32_SUB_FUNC_ENTRY(0x00, 0x01, RV_INS_TYPE_I1, execute_csr, "ebreak"),
};

rv32_opcode_sub_fn_t load_sub_list[] = {
    RV32_SUB_FUNC_ENTRY(0x00, 0x00, RV_INS_TYPE_I1, execute_load, "lb\t$rd, $i($rs1)"),
    RV32_SUB_FUNC_ENTRY(0x01, 0x00, RV_INS_TYPE_I1, execute_load, "lh\t$rd, $i($rs1)"),
    RV32_SUB_FUNC_ENTRY(0x02, 0x00, RV_INS_TYPE_I1, execute_load, "lw\t$rd, $i($rs1)"),
    RV32_SUB_FUNC_ENTRY(0x04, 0x00, RV_INS_TYPE_I1, execute_load, "lbu\t$rd, $i($rs1)"),
    RV32_SUB_FUNC_ENTRY(0x05, 0x00, RV_INS_TYPE_I1, execute_load, "lhu\t$rd, $i($rs1)"),
};

rv32_opcode_sub_fn_t branch_sub_list[] = {
    RV32_SUB_FUNC_ENTRY(0x00, 0x00, RV_INS_TYPE_B, execute_branch, "beq\t$rs1, $rs2, $i"),
    RV32_SUB_FUNC_ENTRY(0x01, 0x00, RV_INS_TYPE_B, execute_branch, "bne\t$rs1, $rs2, $i"),
    RV32_SUB_FUNC_ENTRY(0x04, 0x00, RV_INS_TYPE_B, execute_branch, "blt\t$rs1, $rs2, $i"),
    RV32_SUB_FUNC_ENTRY(0x05, 0x00, RV_INS_TYPE_B, execute_branch, "bge\t$rs1, $rs2, $i"),
    RV32_SUB_FUNC_ENTRY(0x06, 0x00, RV_INS_TYPE_B, execute_branch, "bltu\t$rs1, $rs2, $i"),
    RV32_SUB_FUNC_ENTRY(0x07, 0x00, RV_INS_TYPE_B, execute_branch, "bgeu\t$rs1, $rs2, $i"),
};

rv32_opcode_reg_t opcode_reg_list[] = {
    RV32_OPCODE_NO_SUB(RV32_OPCODE_JAL, RV_INS_TYPE_J, execute_jal, "jal\t$rd, $i"),
    RV32_OPCODE_NO_SUB(RV32_OPCODE_LUI, RV_INS_TYPE_U, execute_lui, "lui\t$rd, $i"),
    RV32_OPCODE_NO_SUB(RV32_OPCODE_JALR, RV_INS_TYPE_I1, execute_jalr,  "jalr\t$rd, $i($r1)"),
    RV32_OPCODE_NO_SUB(RV32_OPCODE_AUIPC, RV_INS_TYPE_U, execute_auipc, "auipc\t$rd, $i"),
    RV32_OPCODE_WITH_SUB(RV32_OPCODE_CSR, csr_sub_list),
    RV32_OPCODE_WITH_SUB(RV32_OPCODE_ALUI, alui_sub_list),
    RV32_OPCODE_WITH_SUB(RV32_OPCODE_ALUR, alur_sub_list),
    RV32_OPCODE_WITH_SUB(RV32_OPCODE_STORE, store_sub_list),
    RV32_OPCODE_WITH_SUB(RV32_OPCODE_LOAD, load_sub_list),
    RV32_OPCODE_WITH_SUB(RV32_OPCODE_BRANCH, branch_sub_list),
};

size_t opcode_list_len = sizeof(opcode_reg_list) / sizeof(rv32_opcode_reg_t);
