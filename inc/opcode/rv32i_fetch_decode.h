#ifndef _RV32_FETCH_DECODE_
#define _RV32_FETCH_DECODE_
#include "main.h"

<<<<<<< HEAD
void rv32_fetch(ram_t *ram, uint32_t pc);
void rv32_decode(uint32_t word, ram_t *ram);

#endif  /*_RV32_FETCH_DECODE_ */
=======
void rv32_fetch(ram_t *iram);
void rv32_decode(uint32_t word);
int rv32_get_csr_index(uint32_t csr_address);
>>>>>>> 4444fc51cf17f5b6f7f4983b85e3e8f16b554b71
