/*****************************************************************************************
 * SLASH-SIM LICENSE
 * Copyrights (C) <2024>, Pranjal Chanda
 *
 * @file    main.h
 * @brief   Main header
 *****************************************************************************************/
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <logging.h>
#include <inttypes.h>
#include <rv32_err.h>
#include <elf_reader.h>
#include <rv32I_struct.h>
#include <rv32i_fetch_decode.h>
#include <rv32i_execute.h>

typedef struct app_config
{
    const char* elf_path;
    const char* elf_name;
}slash_app_config_t;

extern rv32i_ctx_t *g_rv32i_ctx;
extern rv32i_csr_t g_rv32i_csr;
extern slash_app_config_t g_app_cfg;
extern const char *reg_name_list[];
