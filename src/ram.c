#include "inttypes.h"
#include "ram.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

ram_t g_ram;

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

ram_t * init_ram(size_t ram_size)
{
    g_ram.mem = (uint8_t *)malloc(ram_size);
    g_ram.mem_size = ram_size;
    return &g_ram;
}

void deinit_ram()
{
    free(g_ram.mem);
}

uint32_t ram_load(ram_t *ram, uint32_t addr, uint32_t size)
{
    if (addr >= ram->mem_size)
    {
        printf("[E] : Memory access out of bound\n");
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

void ram_store(ram_t *ram, uint32_t addr, uint32_t size, uint32_t value)
{
    if (addr >= ram->mem_size)
    {
        printf("[E] : Memory access out of bound\n");
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
