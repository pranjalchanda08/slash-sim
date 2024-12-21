/*****************************************************************************************
 * SLASH-SIM LICENSE
 * Copyrights (C) <2024>, Pranjal Chanda
 *
 * @file    bus.h
 * @brief   Bus related declaration
 *****************************************************************************************/

#pragma once

#include <inttypes.h>
#include <stdlib.h>
#include <rv32_err.h>
#include <logging.h>
#include <device_tree.h>

typedef struct bus slash_peripheral_t;
typedef struct peripheral_cfg slash_peripheral_cfg_t;
typedef struct peripheral_api
{
    rv32_err_t (*init)(slash_peripheral_cfg_t *per_cfg);
    rv32_err_t (*deinit)(slash_peripheral_cfg_t *per_cfg);
    rv32_err_t (*load)(size_t addr, size_t len, size_t *bytes);
    rv32_err_t (*store)(size_t addr, size_t len, size_t bytes);
} slash_peripheral_api_t;

struct peripheral_cfg
{
    uint8_t *name;                    /* bus registration name */
    size_t mmio_base;                 /* bus bus address */
    size_t mmio_stride;               /* bus bus length */
    void *args;                       /* Device init args */
    slash_peripheral_api_t *api_list; /* Pre defined API list*/
};

struct bus
{
    slash_peripheral_cfg_t per_cfg; /* Peripgeral config */
    // slash_peripheral_api_t api;   /* bus API callback reg */
    slash_peripheral_t *next;     /* Next bus in the bus LinkedList */
};

rv32_err_t device_tree_register(void);
rv32_err_t device_tree_deregister(void);
rv32_err_t peripheral_deregister(char *name);
rv32_err_t peripheral_register(slash_peripheral_cfg_t *cfg);
rv32_err_t peripheral_exec_load(size_t load_addr, size_t len, size_t *value);
rv32_err_t peripheral_exec_store(size_t load_addr, size_t len, size_t value);

#define SLASH_PERIPHERAL_REG(_name, _api_list)     \
    {                                              \
        .name = #_name,                            \
        .mmio_base = SLASH_DT_##_name##_BASE,      \
        .mmio_stride = SLASH_DT_##_name##_STRIDE,  \
        .args = (void *)(SLASH_DT_##_name##_ARGS), \
        .api_list = _api_list,                     \
    }
