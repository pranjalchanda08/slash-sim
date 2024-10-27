#pragma once

#include "stdio.h"
#include "string.h"
#include "inttypes.h"
#include "ram.h"
#include "elf_reader.h"
#include "rv32I_struct.h"
#include "rv32i_fetch_decode.h"
#include "rv32i_execute.h"
#include "platmem.h"

extern rv32i_ctx_t * g_rv32i_ctx;
extern ram_t g_dram_mem;
extern ram_t g_iram_mem;
extern const char *reg_name_list[];
