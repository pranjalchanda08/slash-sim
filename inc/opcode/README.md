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
