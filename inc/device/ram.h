#pragma once

#include "inttypes.h"
#include "stdlib.h"

typedef struct ram
{
    uint8_t *mem;    // ram memory of RAM_SIZE
    size_t mem_size; // ram memory of RAM_SIZE
} ram_t;

ram_t *init_ram(size_t ram_size);
void deinit_ram();
void ram_store(ram_t *ram, uint32_t addr, uint32_t size, uint32_t value);
uint32_t ram_load(ram_t *ram, uint32_t addr, uint32_t size);
