
# Imaginary Assembly Language Assembler

This project implements an assembler that generates machine code files based on an assembly language program file provided.

It aims to offer a tool for understanding and practicing low-level programming concepts without a specific real-world hardware platform or instruction set architecture.

Please note:

* The assembler is designed for an imaginary assembly language, detailed later in this document.

* The implementation does not cover the linkage and loading steps.




## Hardware

* CPU
* RAM (including Stack), with the size of 2^25 bytes.
* Registers:
    - 32 general registers labeled as $0, $1, ..., $31.
    - Each register holds a size of 32 bits.
    - Bit number 0 represents the least significant bit,    bit number 31 represents the most significant bit.

#### Addresses
* The addrsses ranging from 0 to (2^25-1), where each cell size is 1 byte.
*  A memory address is represented by 25 bits as an unsigned number.

#### Notes
* The assembler operates exclusively with positive and negative integers. Arithmetic follows the 2's complement method. 
* The assembler supports characters represented in ASCII code.
## Input File Structure

The input file should be an assembly file (.as extension) consisting of lines up to 80 characters. Each line can fall into one of these categories:

* Empty line: A line containing only spaces or tabs, or no characters at all.
* Comment line: Starts with the ';' character. These lines are ignored by the assembler.
* Directive line: Contains a single directive.
* Instruction line: Contains a single instruction.
## Instructions

Each instruction in the system is comprised of an operation and operands, encoded as 32-bit machine code.  
The instructions occupy four consecutive memory bytes and are stored in little-endian format,
with the memory address referencing the lowest byte.

Each instruction contains an operation code (opcode) that identifies its function,  
 and some instructions have a secondary identification code (funct).

Instructions are categorized into three types: type R, type I, and type J.

#### Instruction Table:

| Name | Type | Structure               | Operation                                            | 
| :--- | :--- | ----------------------- | :----------------------------------------------------| 
| add  | R    | add $rs, $rt, $rd       | R[rd] = R[rs] + R[rt]                                |
| sub  | R    | sub $rs, $rt, $rd       | R[rd] = R[rs] - R[rt]                                |
| and  | R    | and $rs, $rt, $rd       | R[rd] = R[rs] & R[rt]                                |
| or   | R    | or $rs, $rt, $rd        | R[rd] = R[rs] \| R[rt]                               |
| nor  | R    | nor $rs, $rt, $rd       | R[rd] = ~(R[rs] \| R[rt])                            |
| move | R    | move $rs, $rd           | R[rd] = R[rs]                                        |
| mvhi | R    | mvhi $rs, $rd           | R[rd] = R[rs] (bits 16-31)                           |
| mvlo | R    | mvlo $rs, $rd           | R[rd] = R[rs] (bits 0-15)                            |
| addi | I    | addi $rs, immed, $rt    | R[rt] = R[rs] + immed                                |
| subi | I    | subi $rs, immed, $rt    | R[rt] = R[rs] - immed                                |
| andi | I    | andi $rs, immed, $rt    | R[rt] = R[rs] & immed                                |
| ori  | I    | ori $rs, immed, $rt     | R[rt] = R[rs] \| immed                               |
| nori | I    | nori $rs, immed, $rt    | R[rt] = ~(R[rs] \| immed)                            |
| bne  | I    | bne $rs, $rt, label     | if(R[rs] != R[rt]) jump to label                     |
| beq  | I    | beq $rs, $rt, label     | if(R[rs] == R[rt]) jump to label                     |
| blt  | I    | blt $rs, $rt, label     | if(R[rs] < R[rt]) jump to label                      |
| bgt  | I    | bgt $rs, $rt, label     | if(R[rs] > R[rt]) jump to label                      |
| lb   | I    | lb $rs, immed, $r       | (bits 0-7) R[rt]  = Mem[R[rs] + immed] (s = 1byte)   |
| lb   | I    | sb $rs, immed, $r       | Mem[R[rs] + immed] = R[rt] (bit 0-7)                 |
| lw   | I    | lw $rs, immed, $r       | R[rt] = Mem[R[rs] + immed]                           |
| sw   | I    | sw $rs, immed, $r       | Mem[R[rs] + immed] = R[rt]                           |
| lh   | I    | lh $rs, immed, $r       | (bits 0-15) R[rt] = Mem[R[rs] + immed] (s = 2bytes)  |
| sh   | I    | sh $rs, immed, $r       | Mem[R[rs] + immed] = R[rt] (bit 0-15)                |
| jmp  | J    | jmp label               | jump to label                                        |
| jmp  | J    | jmp $rs                 | jump to R[rs]                                        |
| la   | J    | la label                | $0 = label                                           |
| call | J    | call label              | jump to lable, $0 = PC + 4                           |
| stop | J    | stop                    | stop the program                                     | 

*Notes*:
* The instructions names are case sensitive.
* *rd / rt / rs* -   register in the range 0 to 31.
* *R[rd] / R[rt]] / R[rs]* - The content that is stored in the register.
* *immed* -  32-bit integer.
* *label* - label specifying the address (in memory) of an instruction.
* *Mem[_]* - The value found at address _ in memory.
* *(bits x-y)* - The range of bits where the specified value will be stored.
* *s* - the size of the value to be loaded from memory.
* *PC* - the current instruction address.

### Instructions Decode

The instructions are coded into machine code as follows:

Type R instructions:
| opcode   | rs      | rt      | rd      | funct  | unsed |
| :------- | :------ | :------ | :-------| :----- | :---- |
| 31 - 26  | 25 - 21 | 20 - 16 | 15 - 11 | 10 - 6 | 5 - 0 |

 Type Instructions instructions:
| opcode   | rs      | rt      | immed                    |
| :------- | :------ | :------ | :----------------------- |
| 31 - 26  | 25 - 21 | 20 - 16 | 15 - 0                   |

Type J instructions:
| opcode   | reg | address                                |
| :------- | :-- | :------------------------------------- |
| 31 - 26  | 25  | 24 - 0                                 |


## Directives

#### Structure of a directive
* Label (optional)
* The name of the directive
* Operands

### Directive Types
#### Directives `.dh` `.dw` `.db`    
  
The operands of these directives are integers (one or more) with exactly one comma between every two parameters.

This directives instruct the assembler to allocate space in the data image, where the values of the parameters will be stored, and to advance the data counter (DC), according to the number and size of the values.
- Parameters defined by the command statement `.db` Occupies 1 byte.
- Parameters defined by the directive sentence `.dh` Occupies 2 bytes.
- Parameters defined by the directive `.dw` Occupies 4 bytes.

Notes:
- Each number must be of an appropriate size, which will not exceed the representation limits of the defined data type.
- If a label is defined in the instruction sentence, then this label receives the value of the data counter (beforethe promotion), and is inserted into the symbol table.

Example:
```  
.db 7,-57,18, +9
.dw 1200056
XYZ: .dh 0, -6431, 1700, 3, -1
```


#### Directive `.asciz`
The operand of these directive is a single valid *String*.
  
The string's characters are encoded to according to the corresponding ASCII values,  
and are stored in the data image, each character using one byte.
  
To mark the string's end, '0/' is added.  
The assembler's data counter (DC) will be advanced according to the length of the string.  
If a label is included in the line, it gets the data counter's current value (before promotion) and is inserted into the data table.

Example:
```
STR: .asciz "hello world"
```
#### Directive `.entry`
The parameter of this directive is a name of a label defined in the current file.  
The purpose of the `.entry` directive is to characterize this label in a way that allows assembly code found in other files to use it as an instruction operand.

Example:
   ```
   .entry HELLO
   ``` 

Directive: `.extern` 

The parameter of this directive is the name of a label that is not defined in the current file. Its purpose is to notify the assembler that the specified label is defined in another source file and that the current file's assembly code make use of this label.

Example:
   ```
   .extern HELLO
   ```
  
Note: If a label is specified before `.extern`, the assembler ignores it.

## Demo

Inpute file: validTest.as




## Author

#### *Nick Savchenko*

