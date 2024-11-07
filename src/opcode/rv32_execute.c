/*****************************************************************************************
 * SLASH-SIM LICENSE
 * Copyrights (C) <2024>, Pranjal Chanda
 *
 * @file    rv32_execute.c
 * @brief   File to contain the callback function of decoded instructions
 *****************************************************************************************/
#include "rv32i_execute.h"

rv32_err_t execute_lui(exec_args_t *args, size_t *next_pc)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->imm;
    *next_pc = args->c_ctx->pc + RV32_PC_JUMP;
    return RV32_SUCCESS;
}

rv32_err_t execute_auipc(exec_args_t *args, size_t *next_pc)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->pc + args->imm;
    *next_pc = args->c_ctx->pc + RV32_PC_JUMP;
    return RV32_SUCCESS;
}
rv32_err_t execute_jal(exec_args_t *args, size_t *next_pc)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->pc + RV32_PC_JUMP;
    *next_pc = args->c_ctx->pc + args->imm;
    return RV32_SUCCESS;
}

rv32_err_t execute_jalr(exec_args_t *args, size_t *next_pc)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->pc + RV32_PC_JUMP;
    *next_pc = (args->imm + args->c_ctx->cpu_r_u.xn[args->rs1]) & ~(1 << 0);
    return RV32_SUCCESS;
}

rv32_err_t execute_alur_r_add(exec_args_t *args, size_t *next_pc)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->cpu_r_u.xn[args->rs1] + args->c_ctx->cpu_r_u.xn[args->rs2];
    *next_pc = args->c_ctx->pc + RV32_PC_JUMP;
    return RV32_SUCCESS;
}

rv32_err_t execute_alur_r_sub(exec_args_t *args, size_t *next_pc)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->cpu_r_u.xn[args->rs1] - args->c_ctx->cpu_r_u.xn[args->rs2];
    *next_pc = args->c_ctx->pc + RV32_PC_JUMP;
    return RV32_SUCCESS;
}

rv32_err_t execute_alur_r_sll(exec_args_t *args, size_t *next_pc)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->cpu_r_u.xn[args->rs1] << args->c_ctx->cpu_r_u.xn[args->rs2] % RV32_XLEN_BIT;
    *next_pc = args->c_ctx->pc + RV32_PC_JUMP;
    return RV32_SUCCESS;
}

rv32_err_t execute_alur_r_slt(exec_args_t *args, size_t *next_pc)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = (int32_t)args->c_ctx->cpu_r_u.xn[args->rs1] < (int32_t)args->c_ctx->cpu_r_u.xn[args->rs2] ? 1 : 0;
    *next_pc = args->c_ctx->pc + RV32_PC_JUMP;
    return RV32_SUCCESS;
}

rv32_err_t execute_alur_r_sltu(exec_args_t *args, size_t *next_pc)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->cpu_r_u.xn[args->rs1] < args->c_ctx->cpu_r_u.xn[args->rs2] ? 1 : 0;
    *next_pc = args->c_ctx->pc + RV32_PC_JUMP;
    return RV32_SUCCESS;
}

rv32_err_t execute_alur_r_xor(exec_args_t *args, size_t *next_pc)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->cpu_r_u.xn[args->rs1] ^ args->c_ctx->cpu_r_u.xn[args->rs2];
    *next_pc = args->c_ctx->pc + RV32_PC_JUMP;
    return RV32_SUCCESS;
}

rv32_err_t execute_alur_r_srl(exec_args_t *args, size_t *next_pc)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->cpu_r_u.xn[args->rs1] >> args->c_ctx->cpu_r_u.xn[args->rs2] % RV32_XLEN_BIT;
    *next_pc = args->c_ctx->pc + RV32_PC_JUMP;
    return RV32_SUCCESS;
}

rv32_err_t execute_alur_r_sra(exec_args_t *args, size_t *next_pc)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->cpu_r_u.xn[args->rs1] >> args->c_ctx->cpu_r_u.xn[args->rs2] % RV32_XLEN_BIT;
    *next_pc = args->c_ctx->pc + RV32_PC_JUMP;
    return RV32_SUCCESS;
}

rv32_err_t execute_alur_r_or(exec_args_t *args, size_t *next_pc)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->cpu_r_u.xn[args->rs1] | args->c_ctx->cpu_r_u.xn[args->rs2];
    *next_pc = args->c_ctx->pc + RV32_PC_JUMP;
    return RV32_SUCCESS;
}

rv32_err_t execute_alur_r_and(exec_args_t *args, size_t *next_pc)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->cpu_r_u.xn[args->rs1] & args->c_ctx->cpu_r_u.xn[args->rs2];
    *next_pc = args->c_ctx->pc + RV32_PC_JUMP;
    return RV32_SUCCESS;
}

rv32_err_t execute_alui_i1_add(exec_args_t *args, size_t *next_pc)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->cpu_r_u.xn[args->rs1] + args->imm;
    *next_pc = args->c_ctx->pc + RV32_PC_JUMP;
    return RV32_SUCCESS;
}

rv32_err_t execute_alui_i1_slti(exec_args_t *args, size_t *next_pc)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = (int32_t)args->c_ctx->cpu_r_u.xn[args->rs1] < (int32_t)args->imm ? 1 : 0;
    *next_pc = args->c_ctx->pc + RV32_PC_JUMP;
    return RV32_SUCCESS;
}

rv32_err_t execute_alui_i1_sltiu(exec_args_t *args, size_t *next_pc)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->cpu_r_u.xn[args->rs1] < args->imm ? 1 : 0;
    *next_pc = args->c_ctx->pc + RV32_PC_JUMP;
    return RV32_SUCCESS;
}

rv32_err_t execute_alui_i1_xori(exec_args_t *args, size_t *next_pc)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->cpu_r_u.xn[args->rs1] ^ args->imm;
    *next_pc = args->c_ctx->pc + RV32_PC_JUMP;
    return RV32_SUCCESS;
}

rv32_err_t execute_alui_i1_ori(exec_args_t *args, size_t *next_pc)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->cpu_r_u.xn[args->rs1] | args->imm;
    *next_pc = args->c_ctx->pc + RV32_PC_JUMP;
    return RV32_SUCCESS;
}

rv32_err_t execute_alui_i1_andi(exec_args_t *args, size_t *next_pc)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->cpu_r_u.xn[args->rs1] & args->imm;
    *next_pc = args->c_ctx->pc + RV32_PC_JUMP;
    return RV32_SUCCESS;
}

rv32_err_t execute_alui_i2_slli(exec_args_t *args, size_t *next_pc)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->cpu_r_u.xn[args->rs1] << args->imm;
    *next_pc = args->c_ctx->pc + RV32_PC_JUMP;
    return RV32_SUCCESS;
}

rv32_err_t execute_alui_i2_srli(exec_args_t *args, size_t *next_pc)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->cpu_r_u.xn[args->rs1] >> args->imm;
    *next_pc = args->c_ctx->pc + RV32_PC_JUMP;
    return RV32_SUCCESS;
}

rv32_err_t execute_store_s_sb(exec_args_t *args, size_t *next_pc)
{
    rv32_err_t err;
    uint8_t value = args->rs2 & 0x000000FF;
    err = peripheral_exec_store(args->c_ctx->cpu_r_u.xn[args->rs1] + args->imm, sizeof(uint8_t), &value);
    RV32_ASSERT_ERR(err == RV32_SUCCESS, err);
    *next_pc = args->c_ctx->pc + RV32_PC_JUMP;
    return RV32_SUCCESS;
}

rv32_err_t execute_store_s_sh(exec_args_t *args, size_t *next_pc)
{
    rv32_err_t err;
    uint16_t value = args->rs2 & 0x0000FFFF;
    err = peripheral_exec_store(args->c_ctx->cpu_r_u.xn[args->rs1] + args->imm, sizeof(uint16_t), (uint8_t *)&value);
    RV32_ASSERT_ERR(err == RV32_SUCCESS, err);
    *next_pc = args->c_ctx->pc + RV32_PC_JUMP;
    return RV32_SUCCESS;
}

rv32_err_t execute_store_s_sw(exec_args_t *args, size_t *next_pc)
{
    rv32_err_t err;
    uint32_t value = args->rs2 & 0xFFFFFFFF;
    err = peripheral_exec_store(args->c_ctx->cpu_r_u.xn[args->rs1] + args->imm, sizeof(uint16_t), (uint8_t *)&value);
    RV32_ASSERT_ERR(err == RV32_SUCCESS, err);
    *next_pc = args->c_ctx->pc + RV32_PC_JUMP;
    return RV32_SUCCESS;
}

rv32_err_t execute_csr(exec_args_t *args, size_t *next_pc)
{
    uint32_t csr_address = args->imm;
    uint32_t rs1_value = args->c_ctx->cpu_r_u.xn[args->rs1];
    switch (args->fn3)
    {
    case 0x01:
        *(args->csr_ctx[args->csr_index].value) = rs1_value;
        break;
    case 0x02:
        args->c_ctx->cpu_r_u.xn[args->rd] = *(args->csr_ctx[args->csr_index].value);
        break;
    case 0x03:
        *(args->csr_ctx[args->csr_index].value) &= ~rs1_value;
        break;
    }
    *next_pc = args->c_ctx->pc + RV32_PC_JUMP;
    return RV32_SUCCESS;
}

rv32_err_t execute_load(exec_args_t *args, size_t *next_pc)
{
    size_t load_val;
    switch (args->fn3)
    {
    case 0x00:
        /* lb (rd = mmio8(rs1+imm_1)) */
        peripheral_exec_load(args->c_ctx->cpu_r_u.xn[args->rs1] + args->imm, sizeof(uint8_t), (uint8_t *)&load_val);
        args->c_ctx->cpu_r_u.xn[args->rd] = 0xFFFFFF00;
        args->c_ctx->cpu_r_u.xn[args->rd] |= load_val;
        break;
    case 0x04:
        /* lbu (rd = mmio8(rs1+imm_1)) */
        peripheral_exec_load(args->c_ctx->cpu_r_u.xn[args->rs1] + args->imm, sizeof(uint8_t), (uint8_t *)&load_val);
        args->c_ctx->cpu_r_u.xn[args->rd] = 0x00000000;
        args->c_ctx->cpu_r_u.xn[args->rd] |= load_val;
        break;
    case 0x01:
        /* lh (rd = mmio16(rs1+imm_1)) */
        peripheral_exec_load(args->c_ctx->cpu_r_u.xn[args->rs1] + args->imm, sizeof(uint16_t), (uint8_t *)&load_val);
        args->c_ctx->cpu_r_u.xn[args->rd] = 0xFFFFFF00;
        args->c_ctx->cpu_r_u.xn[args->rd] |= load_val;
        break;
    case 0x05:
        /* lhu (rd = mmio16(rs1+imm_1)) */
        peripheral_exec_load(args->c_ctx->cpu_r_u.xn[args->rs1] + args->imm, sizeof(uint16_t), (uint8_t *)&load_val);
        args->c_ctx->cpu_r_u.xn[args->rd] = 0x00000000;
        args->c_ctx->cpu_r_u.xn[args->rd] |= load_val;
        break;
    case 0x02:
        /* lw (rd = mmio16(rs1+imm_1)) */
        peripheral_exec_load(args->c_ctx->cpu_r_u.xn[args->rs1] + args->imm, sizeof(uint32_t), (uint8_t *)&load_val);
        args->c_ctx->cpu_r_u.xn[args->rd] = 0xFFFFFF00;
        args->c_ctx->cpu_r_u.xn[args->rd] |= load_val;
        break;
    default:
        break;
    }
    *next_pc = args->c_ctx->pc + RV32_PC_JUMP;
    return RV32_SUCCESS;
}

rv32_err_t execute_branch(exec_args_t *args, size_t *next_pc)
{
    uint32_t ret_pc = args->c_ctx->pc;
    switch (args->fn3)
    {
    case 0x00:
        /* beq (pc = pc + (rs1==rs2 ? imm_i : 4)*/
        ret_pc = ret_pc +
                 (args->c_ctx->cpu_r_u.xn[args->rs1] == args->c_ctx->cpu_r_u.xn[args->rs2]
                      ? args->imm
                      : RV32_PC_JUMP);
        break;
    case 0x01:
        /* bne (pc = pc + (rs1!=rs2 ? imm_i : 4)*/
        ret_pc = ret_pc +
                 (args->c_ctx->cpu_r_u.xn[args->rs1] != args->c_ctx->cpu_r_u.xn[args->rs2]
                      ? args->imm
                      : RV32_PC_JUMP);
        break;
    case 0x04:
        /* blt (pc = pc + (rs1<rs2 ? imm_i : 4)*/
        ret_pc = ret_pc +
                 ((int32_t)args->c_ctx->cpu_r_u.xn[args->rs1] < (int32_t)args->c_ctx->cpu_r_u.xn[args->rs2]
                      ? args->imm
                      : RV32_PC_JUMP);
        break;
    case 0x05:
        /* bgt (pc = pc + (rs1>=rs2 ? imm_i : 4)*/
        ret_pc = ret_pc +
                 ((int32_t)args->c_ctx->cpu_r_u.xn[args->rs1] >= (int32_t)args->c_ctx->cpu_r_u.xn[args->rs2]
                      ? args->imm
                      : RV32_PC_JUMP);
        break;
    case 0x06:
        /* bltu (pc = pc + (rs1<rs2 ? imm_i : 4)*/
        ret_pc = ret_pc +
                 (args->c_ctx->cpu_r_u.xn[args->rs1] < args->c_ctx->cpu_r_u.xn[args->rs2]
                      ? args->imm
                      : RV32_PC_JUMP);
        break;
    case 0x07:
        /* bgtu (pc = pc + (rs1>=rs2 ? imm_i : 4)*/
        ret_pc = ret_pc +
                 (args->c_ctx->cpu_r_u.xn[args->rs1] >= args->c_ctx->cpu_r_u.xn[args->rs2]
                      ? args->imm
                      : RV32_PC_JUMP);
        break;
    default:
        break;
    }
    *next_pc = ret_pc;
    return RV32_SUCCESS;
}
