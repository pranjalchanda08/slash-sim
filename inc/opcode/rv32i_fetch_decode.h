#ifndef _RV32_FETCH_DECODE_
#define _RV32_FETCH_DECODE_
#include "main.h"

void rv32_fetch(ram_t *ram, uint32_t pc);
void rv32_decode(uint32_t word, ram_t *ram);

#endif  /*_RV32_FETCH_DECODE_ */