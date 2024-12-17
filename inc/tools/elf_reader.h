/*****************************************************************************************
 * SLASH-SIM LICENSE
 * Copyrights (C) <2024>, Pranjal Chanda
 *
 * @file    elf_reader.h
 * @brief   Functions declaration related to elf reader
 *****************************************************************************************/
#pragma once

#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <bus.h>

typedef struct {
    char *name;
    size_t offset;
    size_t size;
    size_t addr;
    bool loadable;
} rv_elf_section_info;

int read_elf(FILE *file, size_t *entry_point);
