/*****************************************************************************************
 * SLASH-SIM LICENSE
 * Copyrights (C) <2024>, Pranjal Chanda
 *
 * @file    rv32_fetch_decode.h
 * @brief   Functions declaration related to Instruction fetch and decode
 *****************************************************************************************/
#pragma once

#include <main.h>

void rv32_fetch(uint32_t pc);
void rv32_decode(uint32_t word);
int rv32_get_csr_index(uint32_t csr_address);
