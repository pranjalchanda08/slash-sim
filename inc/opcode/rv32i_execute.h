#pragma once

#include "main.h"

uint32_t execute_lui(exec_args_t * args);
uint32_t execute_jal(exec_args_t * args);
uint32_t execute_csr(exec_args_t * args);
uint32_t execute_jalr(exec_args_t * args);

uint32_t execute_alui_i1_add (exec_args_t * args);
uint32_t execute_alui_i1_slti (exec_args_t * args);
uint32_t execute_alui_i1_sltiu (exec_args_t * args);
uint32_t execute_alui_i1_xori (exec_args_t * args);
uint32_t execute_alui_i1_ori (exec_args_t * args);
uint32_t execute_alui_i1_andi (exec_args_t * args);
uint32_t execute_alui_i2_slli (exec_args_t * args);
uint32_t execute_alui_i2_srli (exec_args_t * args);

uint32_t execute_alur_r_add(exec_args_t * args);
uint32_t execute_alur_r_sub(exec_args_t * args);
uint32_t execute_alur_r_sll(exec_args_t * args);
uint32_t execute_alur_r_slt(exec_args_t * args);
uint32_t execute_alur_r_sltu(exec_args_t * args);
uint32_t execute_alur_r_xor(exec_args_t * args);
uint32_t execute_alur_r_srl(exec_args_t * args);
uint32_t execute_alur_r_sra(exec_args_t * args);
uint32_t execute_alur_r_or(exec_args_t * args);
uint32_t execute_alur_r_and(exec_args_t * args);

uint32_t execute_store_s_sb(exec_args_t *args);
uint32_t execute_store_s_sh(exec_args_t *args);
uint32_t execute_store_s_sw(exec_args_t *args);

uint32_t execute_load(exec_args_t * args);
uint32_t execute_auipc(exec_args_t * args);
uint32_t execute_branch(exec_args_t * args);