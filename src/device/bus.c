/*****************************************************************************************
 * SLASH-SIM LICENSE
 * Copyrights (C) <2024>, Pranjal Chanda
 *
 * @file    bus.c
 * @brief   Functions related to abstract bus conneced to address bus
 *****************************************************************************************/
/*****************************************************************************************
 * INCLUDES
 *****************************************************************************************/
#include <bus.h>
#include <string.h>

/*****************************************************************************************
 * STATIC VERIABLES
 *****************************************************************************************/
static slash_peripheral_t *peripheral_head;

extern const slash_peripheral_cfg_t splash_dev_tree[];
extern const size_t device_tree_size;

volatile uint32_t bus_reg_max_addr = 0;
/*****************************************************************************************
 * FUNCTION DEFINATION
 *****************************************************************************************/

/**
 * @brief Parse and register devices from device tree
 *
 * @return rv32_err_t
 */
rv32_err_t device_tree_register(void)
{
    rv32_err_t err;
    for (size_t i = 0; i < device_tree_size; i++)
    {
        if (!((splash_dev_tree[i].mmio_base >= bus_reg_max_addr) || (splash_dev_tree[i].mmio_base == 0 && bus_reg_max_addr == 0)))
        {
            LOG_FATAL("[BUS]: Overlappingmemory address for \"%s\"", splash_dev_tree[i].name);
            return RV32_ERR_PERI_BUS_ERROR;
        }
        bus_reg_max_addr = splash_dev_tree[i].mmio_base + splash_dev_tree[i].mmio_stride;
        err = peripheral_register((slash_peripheral_cfg_t *)&splash_dev_tree[i]);
        RV32_ASSERT(err);
    }
    return RV32_SUCCESS;
}

rv32_err_t device_tree_deregister(void)
{
    rv32_err_t err;
    for (size_t i = 0; i < device_tree_size; i++)
    {
        err = peripheral_deregister((char *)splash_dev_tree[i].name);
        RV32_ASSERT(err);
    }
    return RV32_SUCCESS;
}

/**
 * @brief Execuete a load instruction over the bus
 *
 * @param[in] load_addr     Address to perform MMIO load
 * @param[in] len           Length of value to be loaded
 * @param[out] value        Placeholder for reading the value
 * @return rv32_err_t
 */
rv32_err_t peripheral_exec_load(size_t load_addr, size_t len, size_t *value)
{
    rv32_err_t err = RV32_SUCCESS;
    uint8_t peripheral_found = 0;
    slash_peripheral_t *fd = peripheral_head;
    RV32_ASSERT_ERR(fd != NULL, RV32_ERR_PERI_BUS_ERROR);

    while (fd != NULL)
    {
        if ((load_addr >= fd->per_cfg.mmio_base) && (load_addr <= (fd->per_cfg.mmio_base + fd->per_cfg.mmio_stride)))
        {
            err = fd->api.load((load_addr - fd->per_cfg.mmio_base), len, value);
            return err;
        }
        fd = fd->next;
    }
    RV32_ASSERT_ERR(peripheral_found, RV32_ERR_PERI_BUS_ERROR);
}

/**
 * @brief Execute a store instruction on a MMIO device over the bus
 *
 * @param[in] store_addr    MMIO address to tore to
 * @param[in] len           Length of store value
 * @param[in] value         Value to be stored
 * @return rv32_err_t
 */
rv32_err_t peripheral_exec_store(size_t store_addr, size_t len, size_t value)
{
    rv32_err_t err = RV32_SUCCESS;
    uint8_t peripheral_found = 0;
    slash_peripheral_t *fd = peripheral_head;
    RV32_ASSERT_ERR(fd != NULL, RV32_ERR_PERI_BUS_ERROR);

    while (fd != NULL)
    {
        if ((store_addr >= fd->per_cfg.mmio_base) && (store_addr <= (fd->per_cfg.mmio_base + fd->per_cfg.mmio_stride)))
        {
            err = fd->api.store((store_addr - fd->per_cfg.mmio_base), len, value);
            return err;
        }
        fd = fd->next;
    }
    RV32_ASSERT_ERR(peripheral_found, RV32_ERR_PERI_BUS_ERROR);
}

/**
 * @brief Register a bus to the bus
 *
 * @param[in] cfg       Pointer to registration config
 *
 * @return rv32_err_t
 */
rv32_err_t peripheral_register(slash_peripheral_cfg_t *cfg)
{
    rv32_err_t err = RV32_SUCCESS;
    slash_peripheral_t *peripheral_ptr = peripheral_head;
    if (peripheral_head == NULL)
    {
        /* first Node */
        peripheral_head = (slash_peripheral_t *)malloc(sizeof(slash_peripheral_t));
        if (!peripheral_head)
        {
            RV32_ASSERT(RV32_ERR_MALLOC);
        }
        peripheral_ptr = peripheral_head;
    }
    else
    {
        while (peripheral_ptr->next != NULL)
        {
            peripheral_ptr = peripheral_ptr->next;
        }
        peripheral_ptr->next = (slash_peripheral_t *)malloc(sizeof(slash_peripheral_t));
        if (!peripheral_ptr)
        {
            RV32_ASSERT(RV32_ERR_MALLOC);
        }
        peripheral_ptr = peripheral_ptr->next;
    }

    /* Fill all params */
    peripheral_ptr->per_cfg.name = cfg->name;
    peripheral_ptr->per_cfg.mmio_base = cfg->mmio_base;
    peripheral_ptr->per_cfg.mmio_stride = cfg->mmio_stride;
    peripheral_ptr->per_cfg.args = cfg->args;
    peripheral_ptr->api.init = cfg->api_list->init;
    peripheral_ptr->api.load = cfg->api_list->load;
    peripheral_ptr->api.store = cfg->api_list->store;
    peripheral_ptr->api.deinit = cfg->api_list->deinit;

    LOG_DEBUG("[BUS]: Added \"%s\" to the address bus: 0x%x, 0x%x",
              peripheral_ptr->per_cfg.name,
              peripheral_ptr->per_cfg.mmio_base,
              peripheral_ptr->per_cfg.mmio_stride);

    if (peripheral_ptr->api.init)
    {
        /* Call Init callback */
        err = peripheral_ptr->api.init(&peripheral_ptr->per_cfg);
    }

    return err;
}

/**
 * @brief Deregister a peripheral by its name.
 *
 * @param[in] name   Name of the peripheral to be deregistered.
 * @return rv32_err_t
 */
rv32_err_t peripheral_deregister(char *name)
{
    slash_peripheral_t *current = peripheral_head;
    slash_peripheral_t *previous = NULL;

    while (current != NULL)
    {
        if (strcmp(current->per_cfg.name, name) == 0)
        {
            if (current->api.deinit)
            {
                current->api.deinit(&current->per_cfg);
            }
            if (previous == NULL)
            {
                // The peripheral to remove is the head
                peripheral_head = current->next;
            }
            else
            {
                previous->next = current->next;
            }
            free(current);
            LOG_DEBUG("[BUS]: Peripheral \"%s\" deregistered successfully", name);
            return RV32_SUCCESS;
        }
        previous = current;
        current = current->next;
    }
    LOG_DEBUG("[BUS]: Peripheral \"%s\" not found for deregistration", name);
    return RV32_ERR_PERI_NOT_FOUND;
}
