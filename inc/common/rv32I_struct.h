/*****************************************************************************************
 * SLASH-SIM LICENSE
 * Copyrights (C) <2024>, Pranjal Chanda
 *
 * @file    rv32i_struct.h
 * @brief   Structure defination of RV32 Instructions
 *****************************************************************************************/
#pragma once

#include <inttypes.h>
#include <plat_const.h>
#include <opcodes.h>
#include <rv32_csr.h>

#define RV_MASK_U(_wc) (uint32_t)(_wc & 0xFFFFF000)
#define RV_MASK_J(_wc) ((int32_t)(_wc & 0x80000000) >> 11) | (_wc & 0xFF000) | ((_wc >> 9) & 0x800) | ((_wc >> 20) & 0x7FE)
#define RV_MASK_I(_wc) ((int32_t)(_wc & 0xFFF00000) >> 20)
#define RV_MASK_S(_wc) ((int32_t)(_wc & 0xFE000000) >> 20) | ((_wc >> 7) & 0x1F)
#define RV_MASK_B(_wc) ((int32_t)(_wc & 0x80000000) >> 19) | ((_wc & 0x80) << 4) | ((_wc >> 20) & 0x7E0) | ((_wc >> 7) & 0x1E)

typedef enum ins_type
{
    RV_INS_TYPE_U,
    RV_INS_TYPE_J,
    RV_INS_TYPE_R,
    RV_INS_TYPE_I1,
    RV_INS_TYPE_I2,
    RV_INS_TYPE_S,
    RV_INS_TYPE_B
} ins_type_t;

typedef struct _rv32i_ctx
{
    union cpu_r
    {
        uint32_t xn[RV32_XLEN_BIT];
        struct rv32I_gpr_struct
        {
            const uint32_t zero;
            uint32_t ra;
            uint32_t sp;
            uint32_t gp;
            uint32_t tp;
            uint32_t t0;
            uint32_t t1;
            uint32_t t2;
            uint32_t s0;
            uint32_t s1;
            uint32_t a0;
            uint32_t a1;
            uint32_t a2;
            uint32_t a3;
            uint32_t a4;
            uint32_t a5;
            uint32_t a6;
            uint32_t a7;
            uint32_t s2;
            uint32_t s3;
            uint32_t s4;
            uint32_t s5;
            uint32_t s6;
            uint32_t s7;
            uint32_t s8;
            uint32_t s9;
            uint32_t s10;
            uint32_t s11;
            uint32_t t3;
            uint32_t t4;
            uint32_t t5;
            uint32_t t6;
        } cpu_r_s;
    } cpu_r_u;
    uint32_t pc;
} rv32i_ctx_t;

typedef struct _rv_if_u_j
{
    union _wordcode_u
    {
        uint32_t wordcode;
        struct _rv_if_u_struct
        {
            uint32_t opcode : 7;
            uint32_t rd : 5;
            uint32_t imm : 20;
        } _rv_if_u;
    } _wordcode_u;
} rv_if_u_j_t;

typedef struct _rv_if_r
{
    union _wordcode_r
    {
        uint32_t wordcode;
        struct _rv_if_r_struct
        {
            uint32_t opcode : 7;
            uint32_t rd : 5;
            uint32_t fn3 : 3;
            uint32_t rs1 : 5;
            uint32_t rs2 : 5;
            uint32_t fn7 : 7;
        } _rv_if_r;
    } _wordcode_u;
} rv_if_r_t;

typedef struct _rv_if_i1
{
    union _wordcode_i1
    {
        uint32_t wordcode;
        struct _rv_if_i1_struct
        {
            uint32_t opcode : 7;
            uint32_t rd : 5;
            uint32_t fn3 : 3;
            uint32_t rs1 : 5;
            uint32_t imm : 12;
        } _rv_if_i1;
    } _wordcode_u;
} rv_if_i1_t;

typedef struct _rv_if_i2_s_b
{
    union _wordcode_i2_s_b
    {
        uint32_t wordcode;
        struct _rv_if_i2_s_b_struct
        {
            uint32_t opcode : 7;
            uint32_t rd_imm : 5;
            uint32_t fn3 : 3;
            uint32_t rs1 : 5;
            uint32_t shamt_rs2 : 5;
            uint32_t fn7_wc : 7;
        } _rv_if_i2_s_b;
    } _wordcode_u;
} rv_if_i2_s_b_t;
typedef struct {
    uint32_t address;
    uint32_t *value;
} csr_t;

typedef struct exec_args
{
    uint32_t rd;
    uint32_t rs1;
    uint32_t rs2;
    uint32_t imm;
    uint32_t fn3;
    uint32_t fn7;
    ins_type_t ins_type;
    rv32i_ctx_t *c_ctx;
    ram_t * ram;
    csr_t *csr_ctx;
    uint32_t csr_index;
} exec_args_t;

typedef uint32_t (*exec)(exec_args_t *args);

typedef struct rv32_opcode_sub_fn
{
    uint8_t fn3;
    uint16_t fn7;
    exec exec_cb;
    const char *dec_str;
    ins_type_t ins_type;
} rv32_opcode_sub_fn_t;
typedef struct rv32_opcode_reg
{
    ins_type_t ins_type;
    rv32_opcode_t code;
    const char *dec_str;
    uint32_t subfn; // No of sub Functions represented by fn3 or fn7
    exec exec_cb;
    rv32_opcode_sub_fn_t *sub_fn_list;
} rv32_opcode_reg_t;

#define RV32_OPCODE_NO_SUB(_opcode, _type, _cb, _dec_str) { \
    .ins_type = _type,                                      \
    .code = _opcode,                                        \
    .dec_str = _dec_str,                                    \
    .exec_cb = (exec)_cb,                                   \
    .sub_fn_list = NULL}

#define RV32_OPCODE_WITH_SUB(_opcode, _sub_lst) { \
    .code = _opcode,                              \
    .sub_fn_list = _sub_lst,                      \
    .subfn = sizeof(_sub_lst) / sizeof(rv32_opcode_sub_fn_t)}

#define RV32_SUB_FUNC_ENTRY(_fn3, _fn7, _type, _cb, _dec_str) { \
    .fn3 = _fn3,                                                \
    .fn7 = _fn7,                                                \
    .ins_type = _type,                                          \
    .dec_str = _dec_str,\
    .exec_cb = (exec)_cb}
