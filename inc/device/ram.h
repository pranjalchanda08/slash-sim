#pragma once

#include <bus.h>
#include <rv32_err.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <logging.h>

typedef struct ram
{
    uint8_t *mem;    // ram memory of RAM_SIZE
    size_t mem_size; // ram memory of RAM_SIZE
} ram_t;

rv32_err_t init_ram(size_t ram_size);
void deinit_ram();

