/*****************************************************************************************
 * SLASH-SIM LICENSE
 * Copyrights (C) <2024>, Pranjal Chanda
 *
 * @file    bus.h
 * @brief   Bus bus declaration
 *****************************************************************************************/

#pragma once

#include <inttypes.h>
#include <stdlib.h>
#include <rv32_err.h>

typedef struct bus slash_peripheral_t;

typedef struct peripheral_api
{
    rv32_err_t (*load)(size_t addr, size_t len, uint8_t *bytes);
    rv32_err_t (*store)(size_t addr, size_t len, uint8_t *bytes);
} slash_peripheral_api_t;

struct bus
{
    uint8_t *name;                  /* bus registration name */
    size_t mmio_base;               /* bus bus address */
    size_t mmio_stride;             /* bus bus length */
    slash_peripheral_api_t api;     /* bus API callback reg */
    slash_peripheral_t *next;       /* Next bus in the bus LinkedList */
};

rv32_err_t peripheral_register(uint8_t *name,
                               size_t base,
                               size_t stride,
                               slash_peripheral_api_t *api,
                               slash_peripheral_t *fd);

rv32_err_t peripheral_exec_load(size_t load_addr, size_t len, uint8_t *value);
rv32_err_t peripheral_exec_store(size_t store_addr, size_t len, uint8_t *value);
