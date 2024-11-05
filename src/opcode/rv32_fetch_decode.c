/*****************************************************************************************
 * SLASH-SIM LICENSE
 * Copyrights (C) <2024>, Pranjal Chanda
 *
 * @file    rv32_fetch_decode.c
 * @brief   Functions related to Instruction fetch and decode
 *****************************************************************************************/
#include "main.h"

#define MAX_OUTPUT_SIZE 256

extern size_t opcode_list_len;
extern rv32_opcode_reg_t opcode_reg_list[];

const char *reg_name_list[] = {
    "x0 <zero>",
    "x1 <ra>",
    "x2 <sp>",
    "x3 <gp>",
    "x4 <tp>",
    "x5 <t0>",
    "x6 <t1>",
    "x7 <t2>",
    "x8 <s0>",
    "x9 <s1>",
    "x10 <a0>",
    "x11 <a1>",
    "x12 <a2>",
    "x13 <a3>",
    "x14 <a4>",
    "x15 <a5>",
    "x16 <a6>",
    "x17 <a7>",
    "x18 <s2>",
    "x19 <s3>",
    "x20 <s4>",
    "x21 <s5>",
    "x22 <s6>",
    "x23 <s7>",
    "x24 <s8>",
    "x25 <s9>",
    "x26 <s10>",
    "x27 <s11>",
    "x28 <t3>",
    "x29 <t4>",
    "x31 <t5>",
    "x31 <t6>",
    "pc",
};

static const char *csr_reg_list[] = {
    "mstatus",
    "misa",
    "mie",
    "mtvec",
    "mtvt",
    "mstatush",
    "mcountinhibit",
    "mcycle",
    "mhpmevent31",
    "mscratch",
    "mepc",
    "mcause",
    "mtval",
    "mip",
    "mnxti",
    "mintthresh",
    "mscratchcswl",
    "tselect",
    "tdata1",
    "tdata2",
    "tinfo",
    "dcsr",
    "dpc",
    "dscratch0",
    "dscratch1",
    "mcycle",
    "minstret",
};

static csr_t csr_list[] = {
    {RV32_CSR_MSTATUS, &(g_rv32i_csr.mstatus)},
    {RV32_CSR_MISA, &(g_rv32i_csr.misa)},
    {RV32_CSR_MIE, &(g_rv32i_csr.mie)},
    {RV32_CSR_MTVEC, &(g_rv32i_csr.mtvec)},
    {RV32_CSR_MTVT, &(g_rv32i_csr.mtvt)},
    {RV32_CSR_MSTATUSH, &(g_rv32i_csr.mstatush)},
    {RV32_CSR_MCOUNTINHIBIT, &(g_rv32i_csr.mcountinhibit)},
    {RV32_CSR_MCYCLE, &(g_rv32i_csr.mcycle)},
    {RV32_CSR_MHPMEVENT3, &(g_rv32i_csr.mhpmevent31)},
    {RV32_CSR_MSCRATCH, &(g_rv32i_csr.mscratch)},
    {RV32_CSR_MEPC, &(g_rv32i_csr.mepc)},
    {RV32_CSR_MCAUSE, &(g_rv32i_csr.mcause)},
    {RV32_CSR_MTVAL, &(g_rv32i_csr.mtval)},
    {RV32_CSR_MIP, &(g_rv32i_csr.mip)},
    {RV32_CSR_MNXTI, &(g_rv32i_csr.mnxti)},
    {RV32_CSR_MINTTHRESH, &(g_rv32i_csr.mintthresh)},
    {RV32_CSR_MSCRATCHCSWL, &(g_rv32i_csr.mscratchcswl)},
    {RV32_CSR_TSELECT, &(g_rv32i_csr.tselect)},
    {RV32_CSR_TDATA1, &(g_rv32i_csr.tdata1)},
    {RV32_CSR_TDATA2, &(g_rv32i_csr.tdata2)},
    {RV32_CSR_TINFO, &(g_rv32i_csr.tinfo)},
    {RV32_CSR_DCSR, &(g_rv32i_csr.dcsr)},
    {RV32_CSR_DPC, &(g_rv32i_csr.dpc)},
    {RV32_CSR_DSATCH0, &(g_rv32i_csr.dscratch0)},
    {RV32_CSR_DSATCH1, &(g_rv32i_csr.dscratch1)},
    {RV32_CSR_MCYCLE, &(g_rv32i_csr.mcycle)},
    {RV32_CSR_MINSTRET, &(g_rv32i_csr.minstret)},
};

static exec_args_t args;
static union type
{
    uint32_t wordcode;
    rv_if_u_j_t u_j_word;
    rv_if_r_t r_word;
    rv_if_i1_t i1_word;
    rv_if_i2_s_b_t i2_s_b_word;
} type_u;

static uint32_t get_i(uint32_t wc)
{
    wc = RV_MASK_I(wc);
    return wc & (1 << 12) ? wc | 0xFFFFF000 : wc & 0x00000FFF;
}

static void decode_and_print(const char *template, uint32_t rd, uint32_t r1, uint32_t r2, uint32_t imm, uint32_t csr_index, char *formatted)
{
    memset(formatted, 0, MAX_OUTPUT_SIZE); // Initialize the buffer
    const char *ptr = template;
    char *out = formatted;

    while (*ptr)
    {
        if (*ptr == '$')
        { // Found a placeholder
            if (strncmp(ptr, "$rd", 3) == 0)
            {
                out += sprintf(out, "%-10s", reg_name_list[rd]);
                ptr += 3;
            }
            else if (strncmp(ptr, "$rs1", 4) == 0)
            {
                out += sprintf(out, "%-10s", reg_name_list[r1]);
                ptr += 4;
            }
            else if (strncmp(ptr, "$rs2", 4) == 0)
            {
                out += sprintf(out, "%-10s", reg_name_list[r2]);
                ptr += 4;
            }
            else if (strncmp(ptr, "$i", 2) == 0)
            {
                out += sprintf(out, "0x%02x <%02dd>", imm, imm);
                ptr += 2;
            }
            else if (strncmp(ptr, "$csr", 4) == 0)
            {
                out += sprintf(out, "%s", csr_reg_list[csr_index]);
                ptr += 4;
            }
            else
            {
                *out++ = *ptr++; // Copy character as-is
            }
        }
        else
        {
            *out++ = *ptr++; // Copy character as-is
        }
    }
    *out = '\0'; // Null-terminate the string
}

void decode_type(ins_type_t ins_type)
{
    switch (ins_type)
    {
    case RV_INS_TYPE_U:
        args.rd = type_u.u_j_word._wordcode_u._rv_if_u.rd;
        args.imm = RV_MASK_U(type_u.u_j_word._wordcode_u.wordcode);
        break;
    case RV_INS_TYPE_J:
        args.rd = type_u.u_j_word._wordcode_u._rv_if_u.rd;
        args.imm = RV_MASK_J(type_u.u_j_word._wordcode_u.wordcode);
        break;
    case RV_INS_TYPE_R:
        args.rd = type_u.r_word._wordcode_u._rv_if_r.rd;
        args.rs1 = type_u.r_word._wordcode_u._rv_if_r.rs1;
        args.rs2 = type_u.r_word._wordcode_u._rv_if_r.rs2;
        args.fn3 = type_u.r_word._wordcode_u._rv_if_r.fn3;
        args.fn7 = type_u.r_word._wordcode_u._rv_if_r.fn7;
        break;
    case RV_INS_TYPE_S:
        args.rs1 = type_u.i2_s_b_word._wordcode_u._rv_if_i2_s_b.rs1;
        args.rs2 = type_u.i2_s_b_word._wordcode_u._rv_if_i2_s_b.shamt_rs2;
        args.fn3 = type_u.i2_s_b_word._wordcode_u._rv_if_i2_s_b.fn3;
        args.imm = RV_MASK_S(type_u.u_j_word._wordcode_u.wordcode);
        break;
    case RV_INS_TYPE_B:
        args.rs1 = type_u.i2_s_b_word._wordcode_u._rv_if_i2_s_b.rs1;
        args.rs2 = type_u.i2_s_b_word._wordcode_u._rv_if_i2_s_b.shamt_rs2;
        args.fn3 = type_u.i2_s_b_word._wordcode_u._rv_if_i2_s_b.fn3;
        args.imm = RV_MASK_B(type_u.u_j_word._wordcode_u.wordcode);
        break;
    case RV_INS_TYPE_I1:
        args.rs1 = type_u.i1_word._wordcode_u._rv_if_i1.rs1;
        args.rd = type_u.i1_word._wordcode_u._rv_if_i1.rd;
        args.fn3 = type_u.i1_word._wordcode_u._rv_if_i1.fn3;
        args.imm = get_i(type_u.u_j_word._wordcode_u.wordcode);
        break;
    case RV_INS_TYPE_I2:
        args.rs1 = type_u.i2_s_b_word._wordcode_u._rv_if_i2_s_b.rs1;
        args.rd = type_u.i2_s_b_word._wordcode_u._rv_if_i2_s_b.rd_imm;
        args.fn3 = type_u.i2_s_b_word._wordcode_u._rv_if_i2_s_b.fn3;
        args.fn7 = type_u.i2_s_b_word._wordcode_u._rv_if_i2_s_b.fn7_wc;
        args.imm = type_u.i2_s_b_word._wordcode_u._rv_if_i2_s_b.shamt_rs2;
    default:
        break;
    }
}

void rv32_decode(uint32_t word, ram_t *ram)
{
    type_u.wordcode = word;
    const char *dec_str;
    exec exec_cb;
    memset(&args, 0, sizeof(args));
    char formatted[MAX_OUTPUT_SIZE];
    for (size_t i = 0; i < opcode_list_len; i++)
    {
        if (type_u.u_j_word._wordcode_u._rv_if_u.opcode == opcode_reg_list[i].code)
        {
            exec_cb = opcode_reg_list[i].exec_cb;
            if (!opcode_reg_list[i].subfn)
            {
                args.ins_type = opcode_reg_list[i].ins_type;
                decode_type(args.ins_type);
                dec_str = opcode_reg_list[i].dec_str;
            }
            else
            {
                for (size_t j = 0; j < opcode_reg_list[i].subfn; j++)
                {
                    args.ins_type = opcode_reg_list[i].sub_fn_list[j].ins_type;
                    decode_type(args.ins_type);
                    if (opcode_reg_list[i].sub_fn_list[j].fn3 == args.fn3)
                    {
                        if (args.ins_type == RV_INS_TYPE_R || args.ins_type == RV_INS_TYPE_I2)
                        {
                            if (opcode_reg_list[i].sub_fn_list[j].fn7 == args.fn7)
                            {
                                exec_cb = opcode_reg_list[i].sub_fn_list[j].exec_cb;
                                dec_str = opcode_reg_list[i].sub_fn_list[j].dec_str;
                            }
                            else
                                continue;
                        }
                        else
                        {
                            exec_cb = opcode_reg_list[i].sub_fn_list[j].exec_cb;
                            dec_str = opcode_reg_list[i].sub_fn_list[j].dec_str;
                        }
                        break;
                    }
                }
            }
            break;
        }
    }
    uint32_t csr_instruction = (type_u.wordcode >> 20) & 0xFFF;
    args.csr_index = rv32_get_csr_index(csr_instruction);
    args.c_ctx = g_rv32i_ctx;
    args.ram = ram;
    args.csr_ctx = csr_list;
    decode_and_print(dec_str, args.rd, args.rs1, args.rs2, args.imm, args.csr_index, formatted);
    LOG_INFO("[0x%08X]: [PC:0x%08X]:\t%s", type_u.wordcode, g_rv32i_ctx->pc, formatted);
    if (!exec_cb)
    {
        LOG_FATAL("No callback regiterd to perform execution");
        g_rv32i_ctx += RV32_PC_JUMP;
        return;
    }
    g_rv32i_ctx->pc = exec_cb(&args);
}

void rv32_fetch(ram_t *ram, uint32_t pc)
{
    /* Initialise PC */
    g_rv32i_ctx->pc = pc;
    uint32_t wordcode;
    while (1)
    {
        wordcode = ram_load(ram, g_rv32i_ctx->pc, 32);
        if (!wordcode)
        {
            LOG_DEBUG("PC reached EOF");
            break;
        }
        rv32_decode(wordcode, ram);
    }
}

int rv32_get_csr_index(uint32_t csr_address)
{
    size_t num_csrs = sizeof(csr_list) / sizeof(csr_list[0]);
    for (size_t i = 0; i < num_csrs; i++)
    {
        if (csr_list[i].address == csr_address)
        {
            return i;
        }
    }
    return -1;
}
