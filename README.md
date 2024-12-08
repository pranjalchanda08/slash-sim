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
$ make run ARGS=build/add.elf ASM=add
./build/slash_sim build/add.elf add
[10:25:46.083] [DEBUG]: Slash-Sim Startup
[10:25:46.084] [DEBUG]: [BUS]: Added "RAM" to the address bus: 0x0, 0x10000
==================================================
  Section Name   |  Offset  |  Size   | Address
==================================================
.text            | 0x001000 | 0x0001c | 0x00001000
.data            | 0x002000 | 0x00011 | 0x00002000
==================================================
[10:25:46.084] [DEBUG]: Extracted binary data for section: .text
[10:25:46.084] [DEBUG]: Extracted binary data for section: .data
[10:25:46.084] [DEBUG]: Total Memory Layout Size: 0x1011 bytes
[10:25:46.085] [DEBUG]: Reading binary Success. PC: 0x1000

-------------- Execution Start --------------
[10:25:46.085] [INFO ]: [0x00002117]: [PC:0x00001000]:  auipc   x2 <sp>   , 0x2000 <8192d>
[10:25:46.085] [INFO ]: [0x00010113]: [PC:0x00001004]:  addi    x2 <sp>   , x2 <sp>   , 0x00 <00d>
[10:25:46.085] [INFO ]: [0x01300293]: [PC:0x00001008]:  addi    x5 <t0>   , x0 <zero> , 0x13 <19d>
[10:25:46.085] [INFO ]: [0x02300313]: [PC:0x0000100C]:  addi    x6 <t1>   , x0 <zero> , 0x23 <35d>
[10:25:46.085] [INFO ]: [0x006283B3]: [PC:0x00001010]:  add     x7 <t2>   , x5 <t0>   , x6 <t1>
[10:25:46.085] [INFO ]: [0x00001E97]: [PC:0x00001014]:  auipc   x29 <t4>  , 0x1000 <4096d>
[10:25:46.085] [INFO ]: [0xFECE8E93]: [PC:0x00001018]:  addi    x29 <t4>  , x29 <t4>  , 0xffffffec <-20d>
[10:25:46.085] [DEBUG]: PC reached EOF
-------------- Execution End ----------------

[10:25:46.085] [INFO ]: Saving RAM Dump: out/add/ram_dump.bin
[10:25:46.087] [INFO ]: Saving REG Dump: out/add/reg_dump.txt
[10:25:46.087] [DEBUG]: [BUS]: Peripheral "RAM" deregistered successfully
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
$ grep -v "0x00000000" out/add/reg_dump.txt
x2 <sp>: 0x00003000
x5 <t0>: 0x00000013
x6 <t1>: 0x00000023
x7 <t2>: 0x00000036
x29 <t4>: 0x00002000
pc: 0x0000101c
```

