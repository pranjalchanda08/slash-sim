#include "main.h"

#define MAX_OUTPUT_SIZE 256

extern size_t opcode_list_len;
extern rv32_opcode_reg_t opcode_reg_list[];

const char *reg_name_list[] =
    {"x0", "ra", "sp", "gp", "tp", 
     "t0", "t1", "t2", "s0", "s1", 
     "a0", "a1", "a2", "a3", "a4", 
     "a5", "a6", "a7", "s2", "s3", 
     "s4", "s5", "s6", "s7", "s8", 
     "s9", "s10", "s11", "t3", "t4", 
     "t5", "t6"};

const char *csr_reg_list[] = {"mstatus", "misa", "mie",
 "mtvec", "mtvt", "mstatush", "mcountinhibit", "mhpmevent3",
  "mhpmevent31", "mscratch", "mepc", "mcause", "mtval", "mip",
   "mnxti", "mintthresh", "mscratchcswl", "tselect", "tdata1", "tdata2",
    "tinfo", "dcsr", "dpc", "dscratch0", "dscratch1", "mcycle", "minstret"};




static uint32_t get_i(uint32_t wc)
{
    wc = RV_MASK_I(wc);
    return wc & (1 << 12) ? wc | 0xFFFFF000 : wc & 0x00000FFF;
}

static void decode_and_print(const char *template, uint32_t rd, uint32_t r1, uint32_t r2, uint32_t imm , uint32_t csr_index)
{
    char formatted[MAX_OUTPUT_SIZE];
    memset(formatted, 0, MAX_OUTPUT_SIZE); // Initialize the buffer
    const char *ptr = template;
    char *out = formatted;

    while (*ptr)
    {
        if (*ptr == '$')
        { // Found a placeholder
            if (strncmp(ptr, "$rd", 3) == 0)
            {
                out += sprintf(out, "%s", reg_name_list[rd]);
                ptr += 3;
            }
            else if (strncmp(ptr, "$rs1", 4) == 0)
            {
                out += sprintf(out, "%s", reg_name_list[r1]);
                ptr += 4;
            }
            else if (strncmp(ptr, "$rs2", 4) == 0)
            {
                out += sprintf(out, "%s", reg_name_list[r2]);
                ptr += 4;
            }
            else if (strncmp(ptr, "$i", 2) == 0)
            {
                out += sprintf(out, "0x%x", imm);
                ptr += 2;
            }
            else if (strncmp(ptr, "$csr", 4) == 0)
            {
            // int index = 
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
    printf("%s", formatted); // Print the final formatted string
}


static exec_args_t args;
union type
{
    uint32_t wordcode;
    rv_if_u_j_t u_j_word;
    rv_if_r_t r_word;
    rv_if_i1_t i1_word;
    rv_if_i2_s_b_t i2_s_b_word;
} type_u;
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

void rv32_decode(uint32_t word)
{
    type_u.wordcode = word;
    const char *dec_str;
    exec exec_cb;
    memset(&args, 0, sizeof(args)); 
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
    uint32_t csr_instruction= (type_u.wordcode >> 20) & 0xFFF;
    uint32_t csr_index = rv32_get_csr_index(csr_instruction);

    decode_and_print(dec_str, args.rd, args.rs1, args.rs2, args.imm , csr_index);
    args.c_ctx = g_rv32i_ctx;
    if(!exec_cb)
    {
        printf("\n[E]: No callback regiterd to perform execution\n");
        g_rv32i_ctx += RV32_PC_JUMP;
        return;
    }
    g_rv32i_ctx->pc = exec_cb(&args);
}

void rv32_fetch(ram_t *iram)
{
    /* Initialise PC */
    g_rv32i_ctx->pc = iram->base;
    uint32_t wordcode;
    while (1)
    {
        wordcode = ram_load(iram, g_rv32i_ctx->pc, 32);
        printf("\n[0x%08X]: [PC:0x%08X]: ", wordcode, g_rv32i_ctx->pc);
        if (!wordcode)
        {
            printf("PC reached EOF\n");
            break;
        }
        rv32_decode(wordcode);
    }
}


int rv32_get_csr_index(uint32_t csr_address) {

    switch (csr_address) {
        case CSR_MSTATUS:       return 0; // mstatus
        case CSR_MISA:         return 1; // misa
        case CSR_MIE:          return 2; // mie
        case CSR_MTVEC:        return 3; // mtvec
        case CSR_MTVT:         return 4; // mtvt
        case CSR_MSTATUSH:     return 5; // mstatush
        case CSR_MCOUNTINHIBIT: return 6; // mcountinhibit
        case CSR_MHPMEVENT3:   return 7; // mhpmevent3
        case CSR_MHPMEVENT31:  return 8; // mhpmevent31
        case CSR_MSCRATCH:     return 9; // mscratch
        case CSR_MEPC:         return 10; // mepc
        case CSR_MCAUSE:       return 11; // mcause
        case CSR_MTVAL:        return 12; // mtval
        case CSR_MIP:          return 13; // mip
        case CSR_MNXTI:         return 14; // mnxti
        case CSR_MINTTHRESH:   return 15; // mintthresh
        case CSR_MSCRATCHCSWL: return 16; // mscratchcswl
        case CSR_TSELECT:      return 17; // tselect
        case CSR_TDATA1:       return 18; // tdata1
        case CSR_TDATA2:       return 19; // tdata2
        case CSR_TINFO:        return 20; // tinfo
        case CSR_DCSR:         return 21; // dcsr
        case CSR_DPC:          return 22; // dpc
        case CSR_DSATCH0:     return 23; // dscratch0
        case CSR_DSATCH1:     return 24; // dscratch1
        case CSR_MCYCLE:       return 25; // mcycle
        case CSR_MINSTRET:     return 26; // minstret

     // .. i will add those later
        default:               return -1; // Unknown CSR
    }
}

