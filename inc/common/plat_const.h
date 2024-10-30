/*****************************************************************************************
 * SLASH-SIM LICENSE
 * Copyrights (C) <2024>, Pranjal Chanda
 *
 * @file    plat_const.h
 * @brief   Provides platform constants
 *****************************************************************************************/
#pragma once

#define K   *1024

#ifndef RAM_SIZE
#define RAM_SIZE  16 K
#endif

#define RV32_PC_JUMP    sizeof(uint32_t)
#define RV32_XLEN_BIT   32
