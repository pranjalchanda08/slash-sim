/*****************************************************************************************
 * SLASH-SIM LICENSE
 * Copyrights (C) <2024>, Pranjal Chanda
 *
 * @file    peripheral.c
 * @brief   Functions related to abstract peripheral conneced to address bus
 *****************************************************************************************/
/*****************************************************************************************
 * INCLUDES
 *****************************************************************************************/
#include <peripheral.h>

/*****************************************************************************************
 * STATIC VERIABLES
 *****************************************************************************************/
static slash_peripheral_t *peripheral_head;

/*****************************************************************************************
 * FUNCTION DEFINATION
 *****************************************************************************************/
/**
 * @brief Register a peripheral to the bus
 *
 * @param[in] name      Name of the Peripheral
 * @param[in] base      Base address of the peripheral
 * @param[in] stride    Stride length of peripheral
 * @param[in] api       Pointer to API set
 * @param[out] fd       Peripheral descriptor as a return
 *
 * @return rv32_err_t
 */
rv32_err_t register_peripheral(uint8_t *name,
                               size_t base,
                               size_t stride,
                               slash_peripheral_api_t *api,
                               slash_peripheral_t *fd)
{
    RV32_ASSERT_PARAM(fd != NULL);
    RV32_ASSERT_PARAM(name != NULL);

    slash_peripheral_t *peripheral_ptr = peripheral_head;
    if (peripheral_head == NULL)
    {
        /* first Node */
        peripheral_ptr = (slash_peripheral_t *)malloc(sizeof(slash_peripheral_t));
        if (!peripheral_ptr){
            RV32_ASSERT(RV32_ERR_MALLOC);
        }
    }
    else
    {
        while (peripheral_ptr->next != NULL)
        {
            peripheral_ptr = peripheral_ptr->next;
        }
        peripheral_ptr->next = (slash_peripheral_t *)malloc(sizeof(slash_peripheral_t));
        if (!peripheral_ptr){
            RV32_ASSERT(RV32_ERR_MALLOC);
        }
        peripheral_ptr = peripheral_ptr->next;
    }

    /* Fill all params */
    peripheral_ptr->name = name;
    peripheral_ptr->mmio_base = base;
    peripheral_ptr->mmio_stride = stride;
    peripheral_ptr->api.load = api->load;
    peripheral_ptr->api.store = api->store;
    fd = peripheral_ptr;

    return RV32_SUCCESS;
}
