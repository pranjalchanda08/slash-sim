#ifndef _PLATMEM_H_
#define _PLATMEM_H_

#define _K(_x)   (_x*1024)

#ifndef RAM_SIZE
#define RAM_SIZE  _K(16)
#endif

#define RV32_PC_JUMP    sizeof(uint32_t)
#define RV32_XLEN_BIT   32

#endif /* _PLATMEM_H_ */
