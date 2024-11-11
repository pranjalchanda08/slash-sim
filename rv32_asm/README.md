# Example Code

This README explains the functionality of the various RV32 instructions and its correlated example code. 

---

### 1. **Add Instruction**

**Instruction**: `add t2, t0, t1`

The `add` instruction adds the values in registers `t0` and `t1`, and stores the result in register `t2`.

- **Expected Register Output**:
  - `t0 = 19` (0x13)
  - `t1 = 35` (0x23)
  - `t2 = 54` (0x36)

---

### 2. **And Instruction**

**Instruction**: `and t2, t0, t1`

The `and` instruction performs a bitwise AND operation between the values in registers `t0` and `t1`, and stores the result in register `t2`.

- **Explanation**: 
  - The `and` operation compares the binary representation of the two operands, keeping only the bits that are set to `1` in both operands.

- **Expected Register Output**:
  - `t0 = 19` (0x13)
  - `t1 = 2`
  - `t2 = 2` (0x02)

---

### 3. **Auipc Instruction**

**Instruction**: `auipc a0, 1`

The `auipc` instruction adds an immediate value to the current PC and stores the result in a register.

- **Explanation**:
  - The `auipc` instruction computes the address relative to the current PC. The immediate value `1` is shifted left by 12 bits, effectively multiplying it by `0x1000`, and the result is added to the current value of the PC. 

- **Expected Register Output**:
  - `a0 = PC + 0x1000` (Based on current PC)

---

### 4. **BEQ Instructions (branch)**

**Instruction**: `beq a0, a1, branch_here`

The `beq` instruction is a conditional branch that compares two registers. If the values in the two registers are equal, the program branches to the target label.


- **Expected Register Output**:
  - If `a0 = a1 = 5`, `a2` will be set to `1` (since the branch is taken).
  - If the registers were unequal, `a2` would be set to `0` (the branch is not taken).

---

### 5. **CSR Instructions (csrw, csrr)**

**Instruction**: `csrw mscratch, t0` / `csrr t1, mscratch`

The `csrw` and `csrr` instructions read from and write to control and status registers (CSRs) like `mscratch`, `mepc`, and `mtval`.

- **Explanation**: 
  - `csrw` writes the value from a general-purpose register (e.g., `t0`) into a CSR (e.g., `mscratch`).
  - `csrr` reads the value from a CSR (e.g., `mscratch`) into a general-purpose register (e.g., `t1`).

- **Expected Register Output**:
  - `t0 = 0x12345678`
  - After `csrw mscratch, t0`, `mscratch` contains `0x12345678`.
  - After `csrr t1, mscratch`, `t1` will hold the value `0x12345678`.
 
---

### 6. **ITests Instructions (ANDI, SLLI, SRLI)**

**Instructions**: `ANDI`, `SLLI`, `SRLI`

These instructions perform bitwise operations and shifts on register values.

- **Explanation**:
  - `ANDI` performs a bitwise AND between a register and an immediate value.
  - `SLLI` shifts the bits of a register left by a specified number of positions.
  - `SRLI` shifts the bits of a register right by a specified number of positions.

- **Expected Register Output**:
  - For `ANDI a2, a0, 0x0F` with `a0 = 0xF0F0` (binary: `1111 0000 1111 0000`), `a2` becomes `0x0F` (binary: `0000 1111`).
  - After `SLLI a4, a2, 2`, `a4` becomes `0x3C` (shifting `0x0F` left by 2 bits).
  - After `SRLI a5, a2, 3`, `a5` becomes `0x01` (shifting `0x0F` right by 3 bits).

---

### 7. **Jal Instruction**

**Instruction**: `jal ra, function`

The `jal` (Jump and Link) instruction is used for function calls. It stores the return address in the `ra` register and jumps to the target label.

- **Explanation**:
  - The `jal` instruction saves the address of the next instruction (return address) into the `ra` register, and then jumps to the `function` label.

- **Expected Register Output**:
  - `ra` will contain the return address (the address of the instruction after `jal`).
  - `t0` will hold the value `0x12345` after the function is executed.

---

### 8. **Jalr Instruction**

**Instruction**: `jalr ra, 0(t0)`

The `jalr` instruction is similar to `jal` but allows jumping to an address stored in a register, with an optional offset.

- **Expected Register Output**:
  - `ra` will hold the return address.
  - `t1` will be loaded with the value `0x12345` from within the function.

---

### 9. **Lui Instruction**

**Instruction**: `lui t0, 0x12345`

The `lui` (Load Upper Immediate) instruction loads an immediate value into the upper 20 bits of a register, setting the lower 12 bits to zero.

- **Explanation**: 
  - The `lui` instruction sets the upper 20 bits of the register to the provided immediate value and clears the lower 12 bits. 
  
- **Expected Register Output**:
  - After `lui t0, 0x12345`, `t0` will contain `0x12345000`.

---

### 10. **Lw Instruction**

**Instruction**: `lw a3, 0(a2)` / `lhu a1, 0(a0)`

The `lw` and `lhu` instructions are used to load data from memory into registers.

- **Explanation**:
  - `lw` loads a 32-bit word from memory at the address specified by the register `a2`.
  - `lhu` loads a 16-bit halfword from memory at the address specified by `a0` and zero-extends it to 32 bits.

- **Expected Register Output**:
  - After `lw a3, 0(a2)` where `data_val = 0x123`, `a3` will hold `0x123`.
  - After `lhu a1, 0(a0)` with `data_val = 0x123`, `a1` will hold `0x123`.

---

### 11. **Or Instruction**

**Instruction**: `or t2, t0, t1`

The `or` instruction performs a bitwise OR operation between the values in registers `t0` and `t1`, and stores the result in `t2`.
  
- **Expected Register Output**:
  - After `or t2, t0, t1`, where `t0 = 0x13` (binary: `0001 0011`) and `t1 = 0x02` (binary: `0000 0010`), `t2` will contain `0x15` (binary: `0001 0101`).

---

### 12. **Shift Right Instruction (sra)**

**Instruction**: `sra t2, t0, t1`

The `sra` (Shift Right Arithmetic) instruction shifts the bits of the value in `t0` to the right by the number of positions specified in `t1`. It performs an arithmetic shift, preserving the sign bit (most significant bit) for signed values.

- **Expected Register Output**:
  - After `sra t2, t0, t1` with `t0 = 0x13` (binary: `0001 0011`) and `t1 = 2`, `t2` will contain `0x03` (binary: `0000 0011`).

---

### 13. **Shift Right Instruction (srl)**

**Instruction**: `srl t2, t0, t1`

The `srl` (Shift Right Logical) instruction shifts the bits of the value in `t0` to the right by the number of positions specified in `t1`. Unlike `sra`, it does not preserve the sign bit.

- **Expected Register Output**:
  - After `srl t2, t0, t1` with `t0 = 0x13` and `t1 = 2`, `t2` will contain `0x03`.

---

### 14. **Store Byte Instruction (sb)**

**Instruction**: `sb a0, 1(a4)`

The `sb` instruction stores the byte in register `a0` into memory at the address `a4 + 1`.

- **Expected Register Output**:
  - After `sb a0, 1(a4)` with `a0 = 0x13`, the byte `0x13` will be stored at the memory location `a4 + 1`.

---

### 15. **Store Halfword Instruction (sh)**

**Instruction**: `sh a0, 2(a4)`

The `sh` instruction stores the lower 16 bits (halfword) of the value in `a0` to memory at the address `a4 + 2`.

- **Expected Register Output**:
  - After `sh a0, 2(a4)` with `a0 = 0x1234`, the halfword `0x1234` will be stored at the memory location `a4 + 2`.

---

### 16. **Store Word Instruction (sw)**

**Instruction**: `sw a0, 4(a4)`

The `sw` instruction stores the 32-bit word in `a0` to memory at the address `a4 + 4`.

- **Expected Register Output**:
  - After `sw a0, 4(a4)` with `a0 = 0x13`, the word `0x13` will be stored at the memory location `a4 + 4`.

---

### 17. **Sub Instruction**

**Instruction**: `sub t2, t1, t0`

The `sub` instruction subtracts the value in register `t0` from the value in `t1`, and stores the result in register `t2`.

- **Expected Register Output**:
  - `t0 = 0x13` (19 in decimal)
  - `t1 = 0x23` (35 in decimal)
  - `t2 = 0x10` (16 in decimal)

---

### 18. **Xor Instruction**

**Instruction**: `xor t2, t0, t1`

The `xor` instruction performs a bitwise exclusive OR operation between the values in registers `t0` and `t1`, and stores the result in register `t2`.

- **Expected Register Output**:
  - After `xor t2, t0, t1` with `t0 = 0x13` (binary: `0001 0011`) and `t1 = 0x02` (binary: `0000 0010`), `t2` will contain `0x11` (binary: `0001 0001`).

---

## 19. **Set Less Than Instruction (slt)**

**Instruction**: `slt t2, t0, t1`

The `slt` (Set Less Than) instruction sets register `t2` to `1` if the value in `t0` is less than the value in `t1`, and `0` otherwise. It compares the two values as signed integers.

- **Expected Register Output**:
  - After `slt t2, t0, t1` with `t0 = 0x13` (19 in decimal) and `t1 = 0x02` (2 in decimal), since `t0` is greater than `t1`, `t2` will contain `0x00`.

---

### 20. **Set Less Than Unsigned Instruction (sltu)**

**Instruction**: `sltu t2, t0, t1`

The `sltu` (Set Less Than Unsigned) instruction sets register `t2` to `1` if the value in `t0` is less than the value in `t1` when treated as unsigned integers, and `0` otherwise.

- **Explanation**:
  - The `sltu` instruction performs an unsigned comparison: it compares the bit patterns of `t0` and `t1` without considering their sign.

- **Expected Register Output**:
  - After `sltu t2, t0, t1` with `t0 = 0x13` (19 in decimal) and `t1 = 0x02` (2 in decimal), since `t0` is greater than `t1` in an unsigned context, `t2` will contain `0x00`.
