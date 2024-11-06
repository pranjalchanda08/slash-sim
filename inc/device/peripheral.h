/*****************************************************************************************
 * SLASH-SIM LICENSE
 * Copyrights (C) <2024>, Pranjal Chanda
 *
 * @file    peripheral.h
 * @brief   Bus Peripheral declaration
 *****************************************************************************************/

#pragma once

#include <inttypes.h>
#include <stdlib.h>
#include <rv32_err.h>

typedef struct peripheral slash_peripheral_t;

typedef struct peripheral_api
{
    rv32_err_t (*load)(size_t addr, size_t len, uint8_t *bytes);
    rv32_err_t (*store)(size_t addr, size_t len, uint8_t *bytes);
} slash_peripheral_api_t;

struct peripheral
{
    uint8_t *name;                  /* Peripheral registration name */
    size_t mmio_base;               /* Peripheral bus address */
    size_t mmio_stride;             /* Peripheral bus length */
    slash_peripheral_api_t api;     /* Peripheral API callback reg */
    slash_peripheral_t *next;       /* Next Peripheral in the peripheral LinkedList */
};

rv32_err_t register_peripheral(uint8_t *name,
                               size_t base,
                               size_t stride,
                               slash_peripheral_api_t *api,
                               slash_peripheral_t *fd);
