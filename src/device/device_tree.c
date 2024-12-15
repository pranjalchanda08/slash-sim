/*****************************************************************************************
 * SLASH-SIM LICENSE
 * Copyrights (C) <2024>, Pranjal Chanda
 *
 * @file    device_tree.c
 * @brief   Device tree definations
 *****************************************************************************************/
/*****************************************************************************************
 * INCLUDES
 *****************************************************************************************/
#include <bus.h>
#include <ram.h>

/**
 * @brief This is the device tree that is used to register any device to the bus.
 *
 * To add any device define the following in device_tree.h:
 * 1. SLASH_DT_<NAME>_BASE  xxxx
 * 2. SLASH_DT_<NAME>_STRIDE  xxxx
 * 3. SLASH_DT_<NAME>_ARGS  xxxx
 *
 * Finally add the device in the array using SLASH_PERIPHERAL_REG(<NAME>, <&api_list>)
 */
const slash_peripheral_cfg_t slash_dev_tree[] =
{
    SLASH_PERIPHERAL_REG(RAM, &ram_peripheral_api),
};

const size_t device_tree_size = sizeof(slash_dev_tree)/sizeof(slash_peripheral_cfg_t);
