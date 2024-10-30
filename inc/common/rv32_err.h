/*****************************************************************************************
 * SLASH-SIM LICENSE
 * Copyrights (C) <2024>, Pranjal Chanda
 *
 * @file    rv32_err.h
 * @brief   Defination related to rv32_err
 *****************************************************************************************/
#pragma once

typedef enum
{
    RV32_SUCCESS,
    RV32_ERR_ASSERT_PARAM,
    RV32_ERR_RAM_INIT,
    RV32_ERR_ELF_FILE,
    RV32_ERR_MALLOC,
}rv32_err_t;

#define RV32_ASSERT(_x) if(_x != RV32_SUCCESS) return _x
#define RV32_ASSERT_GOTO(_x, _sec) if(_x != RV32_SUCCESS) goto _sec
