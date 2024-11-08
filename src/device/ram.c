/*****************************************************************************************
 * SLASH-SIM LICENSE
 * Copyrights (C) <2024>, Pranjal Chanda
 *
 * @file    ram.c
 * @brief   Functions related to RAM load and store
 *****************************************************************************************/
/*****************************************************************************************
 * INCLUDES
 *****************************************************************************************/
#include <ram.h>
#include <main.h>
/*****************************************************************************************
 * GLOBALS
 *****************************************************************************************/
ram_t g_ram;
slash_peripheral_t ram_fd;

static rv32_err_t ram_peripheral_init(slash_peripheral_cfg_t *per_cfg);
static rv32_err_t ram_peripheral_deinit(slash_peripheral_cfg_t *per_cfg);
static rv32_err_t ram_peripheral_load(size_t addr, size_t len, size_t *bytes);
static rv32_err_t ram_peripheral_store(size_t addr, size_t len, size_t bytes);

static void ram_dump(char const *elf_name, size_t ram_base_addr, size_t ram_size);

slash_peripheral_api_t ram_peripheral_api = {
    .init = ram_peripheral_init,
    .load = ram_peripheral_load,
    .store = ram_peripheral_store,
    .deinit = ram_peripheral_deinit,
};

static rv32_err_t ram_peripheral_init(slash_peripheral_cfg_t *per_cfg)
{
    g_ram.mem = (uint8_t *)calloc((size_t)per_cfg->mmio_stride, 1);
    g_ram.mem_size = (size_t)per_cfg->mmio_stride;
    return RV32_SUCCESS;
}

static rv32_err_t ram_peripheral_deinit(slash_peripheral_cfg_t *per_cfg)
{
    ram_dump(g_app_cfg.elf_name, per_cfg->mmio_base, per_cfg->mmio_stride);
    free(g_ram.mem);
    return RV32_SUCCESS;
}

static uint8_t dram_load_8(ram_t *ram, uint32_t addr)
{
    return (uint32_t)ram->mem[addr];
}

static uint16_t dram_load_16(ram_t *ram, uint32_t addr)
{
    return (uint32_t)ram->mem[addr] | (uint32_t)ram->mem[addr + 1] << 8;
}
static uint32_t dram_load_32(ram_t *ram, uint32_t addr)
{
    return (uint32_t)ram->mem[addr] |
           (uint32_t)ram->mem[addr + 1] << 8 |
           (uint32_t)ram->mem[addr + 2] << 16 |
           (uint32_t)ram->mem[addr + 3] << 24;
}

static void dram_store_8(ram_t *ram, uint32_t addr, uint8_t value)
{
    ram->mem[addr] = value;
}
static void dram_store_16(ram_t *ram, uint32_t addr, uint16_t value)
{
    ram->mem[addr] = (uint8_t)(value & 0xff);
    ram->mem[addr + 1] = (uint8_t)((value >> 8) & 0xff);
}
static void dram_store_32(ram_t *ram, uint32_t addr, uint32_t value)
{
    ram->mem[addr] = (uint8_t)(value & 0xff);
    ram->mem[addr + 1] = (uint8_t)((value >> 8) & 0xff);
    ram->mem[addr + 2] = (uint8_t)((value >> 16) & 0xff);
    ram->mem[addr + 3] = (uint8_t)((value >> 24) & 0xff);
}


static uint32_t ram_load(ram_t *ram, uint32_t addr, uint32_t size)
{
    if (addr >= ram->mem_size)
    {
        LOG_FATAL("Memory access out of bound");
    }

    switch (size)
    {
    case 8:
        return dram_load_8(ram, addr);
        break;
    case 16:
        return dram_load_16(ram, addr);
        break;
    case 32:
        return dram_load_32(ram, addr);
        break;
    default:
        break;
    }
    return 1;
}

static void ram_store(ram_t *ram, uint32_t addr, uint32_t size, uint32_t value)
{
    if (addr >= ram->mem_size)
    {
        LOG_FATAL("Memory access out of bound");
    }

    switch (size)
    {
    case 8:
        dram_store_8(ram, addr, value);
        break;
    case 16:
        dram_store_16(ram, addr, value);
        break;
    case 32:
        dram_store_32(ram, addr, value);
        break;
    default:;
    }
}

static rv32_err_t ram_peripheral_load(size_t addr, size_t len, size_t *bytes)
{
    *bytes = ram_load(&g_ram, addr, (len * 8));
    return RV32_SUCCESS;
}

static rv32_err_t ram_peripheral_store(size_t addr, size_t len, size_t bytes)
{
    ram_store(&g_ram, addr, (len * 8), bytes);
    return RV32_SUCCESS;
}

/*******************************************************************************************
 * @brief Store the current RAM dump of the running program
 *
 * @param[in] elf_name      Name of the program being simulated
 * @param[in] ram_base_addr RAM Base address
 * @param[in] ram_size      RAM Length
 ******************************************************************************************/
static void ram_dump(char const *elf_name, size_t ram_base_addr, size_t ram_size)
{
    uint32_t buff;
    char *out_file_path = malloc(strlen(elf_name) + 20);
    sprintf(out_file_path, "out/%s/ram_dump.bin", elf_name);
    LOG_INFO("Saving RAM Dump: %s", out_file_path);
    FILE *mem = fopen(out_file_path, "wb");
    free(out_file_path);
    while (ram_base_addr < ram_size)
    {
        peripheral_exec_load(ram_base_addr, sizeof(uint8_t), (size_t*)&buff);
        ram_base_addr++;
        fwrite(&buff, sizeof(uint8_t), 1, mem);
    }
    fclose(mem);
}
