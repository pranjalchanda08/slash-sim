/*****************************************************************************************
 * SLASH-SIM LICENSE
 * Copyrights (C) <2024>, Pranjal Chanda
 *
 * @file    ram.h
 * @brief   RAM related declaration
 *****************************************************************************************/
#pragma once

#include <bus.h>
#include <rv32_err.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <logging.h>
#include <plat_const.h>

typedef struct ram
{
    uint8_t *mem;    // ram memory of RAM_SIZE
    size_t mem_size; // ram memory of RAM_SIZE
} ram_t;

extern slash_peripheral_api_t ram_peripheral_api;
