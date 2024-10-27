# Slash Sim

Slash Sim is a simple C based RV32 Instruction set simulator. This can be used to simulate any binary file that is compiled using RISCV toolchain.

## Prerequisits
```sh
$ sudo apt install gcc-riscv64-unknown-elf build-essential make
```

## Build Emmulator
* Build Emulator
```sh
$ make
```
* Clean
```sh
$ make clean
```
* Buid asm
```sh
$ make asm ASM={asm_name}
```

## Run
```sh
$ make run ARGS={path_to_asm.elf} ASM={asm_name}
```

## Example run

#### add.asm
```asm
.align 2

.section .text
.globl _start

_start:
    addi a0, zero, 0x13
    addi a1, zero, 0x23
    add a2, a0, a1
    la a4, msg

.section .data
msg:
    .string "Pranjal\n"
    .string "Chanda\n"

```
#### rv32.ld
```ld
OUTPUT_ARCH("riscv")
OUTPUT_FORMAT("elf32-littleriscv")

MEMORY
{
	/* VM Addresses */
	IRAM (rx)	: ORIGIN = 0x1000, LENGTH = 1k
	DRAM (rwx)	: ORIGIN = 0x2000, LENGTH = 4k
}

ENTRY(_start)
SECTIONS
{
    .text : {
        *(.text)
    } > IRAM
    .data : {
        *(.data)
    } > DRAM
    . = 0x2000 + 4k;
    PROVIDE(stack_top = .);
    _end = .;
}

```
#### Build add.bin and slash_sim
```sh
$ make clean
$ make
$ make asm ASM=add
```
#### Run slash_sim
```sh
$ make run ARGS=build/add.elf ASM=add
```

#### Output
```sh
$ ./build/slash_sim build/add.elf add
Slash-Sim Startup 
RAM Init done
ELF found. Reading build/add.elf
==================================================
  Section Name   |  Offset  |  Size   | Address 
==================================================
.text            | 0x001000 | 0x0001c | 0x00001000
.data            | 0x002000 | 0x00011 | 0x00002000
==================================================
Extracted binary data for section: .text
Extracted binary data for section: .data
Total Memory Layout Size: 0x1011 bytes
Reading binary Success. PC: 0x1000

-------------- Execution Start --------------

[0x00002117]: [PC:0x00001000]: auipc    sp, 0x2000
[0x00010113]: [PC:0x00001004]: addi     sp, sp, 0x0
[0x01300293]: [PC:0x00001008]: addi     t0, x0, 0x13
[0x02300313]: [PC:0x0000100C]: addi     t1, x0, 0x23
[0x006283B3]: [PC:0x00001010]: add      t2, t0, t1
[0x00001E97]: [PC:0x00001014]: auipc    t4, 0x1000
[0xFECE8E93]: [PC:0x00001018]: addi     t4, t4, 0xffffffec
[0x00000000]: [PC:0x0000101C]: PC reached EOF
-------------- Execution End ----------------

Saving RAM Dump: out/add/ram_dump.bin
Saving REG Dump: out/add/reg_dump.txt
RAM De-Init done
```

#### RAM Dump
```sh
$ hexdump out/add/ram_dump.bin
0000000 0000 0000 0000 0000 0000 0000 0000 0000
*
0001000 2117 0000 0113 0001 0293 0130 0313 0230
0001010 83b3 0062 1e97 0000 8e93 fece 0000 0000
0001020 0000 0000 0000 0000 0000 0000 0000 0000
*
0002000 7250 6e61 616a 0a6c 4300 6168 646e 0a61     Pranjal Chanda
0002010 0000 0000 0000 0000 0000 0000 0000 0000
*
0004000
```

#### CPU REG Dump
```sh
grep -v "0x00000000" out/add/reg_dump.txt
  sp: 0x00003000
  t0: 0x00000013
  t1: 0x00000023
  t2: 0x00000036
  t4: 0x00002000
  pc: 0x0000101c

```

### Add an instruction to the build

To add an instruction into the framework we need to do the following:
1. `opcodes.h`: Add an entry into `rv32_opcode_t` if adding a new opcode.
    * The decode string shall follow formated string consisting of `$rd`, `$rs1`, `$rs2`, `$i` only.
    * All immidiate value formats of any instruction structure shall be represented using `$i`.
2. `opcode_tree.c`: Add entry to `opcode_reg_list` using following macros:
    * `RV32_OPCODE_NO_SUB`    : API to add an opcode entry to the list with no sub functionality to the opcode.
    * `RV32_OPCODE_WITH_SUB` : API to add an opcode entry to the list with `n` sub functionality to the opcode.
    * `RV32_SUB_FUNC_ENTRY` : API to add sub functions in an array.
3. `rv32_execute.c` : Add callback entry for the instruction execution.


