#include "rv32i_execute.h"

uint32_t execute_lui(exec_args_t *args)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->imm;
    return args->c_ctx->pc + RV32_PC_JUMP;
}

uint32_t execute_auipc(exec_args_t *args)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->pc + args->imm;
    return args->c_ctx->pc + RV32_PC_JUMP;
}

uint32_t execute_jal(exec_args_t *args)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->pc + RV32_PC_JUMP;
    return args->c_ctx->pc + args->imm;
}

uint32_t execute_jalr(exec_args_t *args)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->pc + RV32_PC_JUMP;
    return args->c_ctx->pc + args->imm + args->c_ctx->cpu_r_u.xn[args->rs1];
}

uint32_t execute_alur_r_add(exec_args_t *args)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->cpu_r_u.xn[args->rs1] + args->c_ctx->cpu_r_u.xn[args->rs2];
    return args->c_ctx->pc + RV32_PC_JUMP;
}

uint32_t execute_alur_r_sub(exec_args_t *args)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->cpu_r_u.xn[args->rs1] - args->c_ctx->cpu_r_u.xn[args->rs2];
    return args->c_ctx->pc + RV32_PC_JUMP;
}

uint32_t execute_alur_r_sll(exec_args_t *args)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->cpu_r_u.xn[args->rs1] << args->c_ctx->cpu_r_u.xn[args->rs2] % RV32_XLEN_BIT;
    return args->c_ctx->pc + RV32_PC_JUMP;
}

uint32_t execute_alur_r_slt(exec_args_t *args)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = (int32_t)args->c_ctx->cpu_r_u.xn[args->rs1] < (int32_t)args->c_ctx->cpu_r_u.xn[args->rs2] ? 1 : 0;
    return args->c_ctx->pc + RV32_PC_JUMP;
}

uint32_t execute_alur_r_sltu(exec_args_t *args)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->cpu_r_u.xn[args->rs1] < args->c_ctx->cpu_r_u.xn[args->rs2] ? 1 : 0;
    return args->c_ctx->pc + RV32_PC_JUMP;
}

uint32_t execute_alur_r_xor(exec_args_t *args)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->cpu_r_u.xn[args->rs1] ^ args->c_ctx->cpu_r_u.xn[args->rs2];
    return args->c_ctx->pc + RV32_PC_JUMP;
}

uint32_t execute_alur_r_srl(exec_args_t *args)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->cpu_r_u.xn[args->rs1] >> args->c_ctx->cpu_r_u.xn[args->rs2] % RV32_XLEN_BIT;
    return args->c_ctx->pc + RV32_PC_JUMP;
}

uint32_t execute_alur_r_sra(exec_args_t *args)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->cpu_r_u.xn[args->rs1] >> args->c_ctx->cpu_r_u.xn[args->rs2] % RV32_XLEN_BIT;
    return args->c_ctx->pc + RV32_PC_JUMP;
}

uint32_t execute_alur_r_or(exec_args_t *args)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->cpu_r_u.xn[args->rs1] | args->c_ctx->cpu_r_u.xn[args->rs2];
    return args->c_ctx->pc + RV32_PC_JUMP;
}

uint32_t execute_alur_r_and(exec_args_t *args)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->cpu_r_u.xn[args->rs1] & args->c_ctx->cpu_r_u.xn[args->rs2];
    return args->c_ctx->pc + RV32_PC_JUMP;
}

uint32_t execute_alui_i1_add(exec_args_t *args)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->cpu_r_u.xn[args->rs1] + args->imm;
    return args->c_ctx->pc + RV32_PC_JUMP;
}

uint32_t execute_alui_i1_slti(exec_args_t *args)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = (int32_t)args->c_ctx->cpu_r_u.xn[args->rs1] < (int32_t)args->imm ? 1 : 0;
    return args->c_ctx->pc + RV32_PC_JUMP;
}

uint32_t execute_alui_i1_sltiu(exec_args_t *args)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->cpu_r_u.xn[args->rs1] < args->imm ? 1 : 0;
    return args->c_ctx->pc + RV32_PC_JUMP;
}

uint32_t execute_alui_i1_xori(exec_args_t *args)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->cpu_r_u.xn[args->rs1] ^ args->imm;
    return args->c_ctx->pc + RV32_PC_JUMP;
}

uint32_t execute_alui_i1_ori(exec_args_t *args)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->cpu_r_u.xn[args->rs1] | args->imm;
    return args->c_ctx->pc + RV32_PC_JUMP;
}

uint32_t execute_alui_i1_andi(exec_args_t *args)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->cpu_r_u.xn[args->rs1] & args->imm;
    return args->c_ctx->pc + RV32_PC_JUMP;
}

uint32_t execute_alui_i2_slli(exec_args_t *args)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->cpu_r_u.xn[args->rs1] << args->imm;
    return args->c_ctx->pc + RV32_PC_JUMP;
}

uint32_t execute_alui_i2_srli(exec_args_t *args)
{
    args->c_ctx->cpu_r_u.xn[args->rd] = args->c_ctx->cpu_r_u.xn[args->rs1] >> args->imm;
    return args->c_ctx->pc + RV32_PC_JUMP;
}

uint32_t execute_store_s_sb(exec_args_t *args)
{
    ram_store(&g_dram_mem, args->c_ctx->cpu_r_u.xn[args->rs1] + args->imm, 8, args->rs2 & 0x000000FF);
    return args->c_ctx->pc + RV32_PC_JUMP;
}

uint32_t execute_store_s_sh(exec_args_t *args)
{
    ram_store(&g_dram_mem, args->c_ctx->cpu_r_u.xn[args->rs1] + args->imm, 16, args->rs2 & 0x0000FFFF);
    return args->c_ctx->pc + RV32_PC_JUMP;
}

uint32_t execute_store_s_sw(exec_args_t *args)
{
    ram_store(&g_dram_mem, args->c_ctx->cpu_r_u.xn[args->rs1] + args->imm, 32, args->rs2 & 0xFFFFFFFF);
    return args->c_ctx->pc + RV32_PC_JUMP;
}

uint32_t execute_csr(exec_args_t *args)
{
    switch (args->fn3)
    {
        /* CSR not supported yet */
    default:
        break;
    }
    return args->c_ctx->pc + RV32_PC_JUMP;
}

uint32_t execute_load(exec_args_t *args)
{
    switch (args->fn3)
    {
    case 0x00:
        /* lb (rd = mmio8(rs1+imm_1)) */
        args->c_ctx->cpu_r_u.xn[args->rd] = 0xFFFFFF00;
        args->c_ctx->cpu_r_u.xn[args->rd] |=
            ram_load(&g_dram_mem,
                     args->c_ctx->cpu_r_u.xn[args->rs1] + args->imm,
                     8);
        break;
    case 0x04:
        /* lbu (rd = mmio8(rs1+imm_1)) */
        args->c_ctx->cpu_r_u.xn[args->rd] = 0x00000000;
        args->c_ctx->cpu_r_u.xn[args->rd] |=
            ram_load(&g_dram_mem,
                     args->c_ctx->cpu_r_u.xn[args->rs1] + args->imm,
                     8);
        break;
    case 0x01:
        /* lh (rd = mmio16(rs1+imm_1)) */
        args->c_ctx->cpu_r_u.xn[args->rd] = 0xFFFFFF00;
        args->c_ctx->cpu_r_u.xn[args->rd] |=
            ram_load(&g_dram_mem,
                     args->c_ctx->cpu_r_u.xn[args->rs1] + args->imm,
                     16);
        break;
    case 0x05:
        /* lhu (rd = mmio16(rs1+imm_1)) */
        args->c_ctx->cpu_r_u.xn[args->rd] = 0x00000000;
        args->c_ctx->cpu_r_u.xn[args->rd] |=
            ram_load(&g_dram_mem,
                     args->c_ctx->cpu_r_u.xn[args->rs1] + args->imm,
                     16);
        break;
    case 0x02:
        /* lw (rd = mmio16(rs1+imm_1)) */
        args->c_ctx->cpu_r_u.xn[args->rd] = 0xFFFFFF00;
        args->c_ctx->cpu_r_u.xn[args->rd] |=
            ram_load(&g_dram_mem,
                     args->c_ctx->cpu_r_u.xn[args->rs1] + args->imm,
                     32);
        break;
    default:
        break;
    }
    return args->c_ctx->pc + RV32_PC_JUMP;
}

uint32_t execute_branch(exec_args_t *args)
{
    uint32_t ret_pc = args->c_ctx->pc;
    switch (args->fn3)
    {
    case 0x00:
        /* beq (pc = pc + (rs1==rs2 ? imm_i : 4)*/
        ret_pc = ret_pc +
                 (args->c_ctx->cpu_r_u.xn[args->rs1] == args->c_ctx->cpu_r_u.xn[args->rs1]
                      ? args->imm : RV32_PC_JUMP);
        break;
    case 0x01:
        /* bne (pc = pc + (rs1!=rs2 ? imm_i : 4)*/
        ret_pc = ret_pc +
                 (args->c_ctx->cpu_r_u.xn[args->rs1] != args->c_ctx->cpu_r_u.xn[args->rs1]
                      ? args->imm : RV32_PC_JUMP);
        break;
    case 0x04:
        /* blt (pc = pc + (rs1<rs2 ? imm_i : 4)*/
        ret_pc = ret_pc +
                 ((int32_t)args->c_ctx->cpu_r_u.xn[args->rs1] < (int32_t)args->c_ctx->cpu_r_u.xn[args->rs1]
                      ? args->imm : RV32_PC_JUMP);
        break;
    case 0x05:
        /* bgt (pc = pc + (rs1>=rs2 ? imm_i : 4)*/
        ret_pc = ret_pc +
                 ((int32_t)args->c_ctx->cpu_r_u.xn[args->rs1] >= (int32_t)args->c_ctx->cpu_r_u.xn[args->rs1]
                      ? args->imm : RV32_PC_JUMP);
        break;
    case 0x06:
        /* bltu (pc = pc + (rs1<rs2 ? imm_i : 4)*/
        ret_pc = ret_pc +
                 (args->c_ctx->cpu_r_u.xn[args->rs1] < args->c_ctx->cpu_r_u.xn[args->rs1]
                      ? args->imm : RV32_PC_JUMP);
        break;
    case 0x07:
        /* bgtu (pc = pc + (rs1>=rs2 ? imm_i : 4)*/
        ret_pc = ret_pc +
                 (args->c_ctx->cpu_r_u.xn[args->rs1] >= args->c_ctx->cpu_r_u.xn[args->rs1]
                      ? args->imm : RV32_PC_JUMP);
        break;
    default:
        break;
    }
    return ret_pc;
}
