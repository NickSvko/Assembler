
# Imaginary Assembly Language Assembler

This program interprets a custom assembly language, converting assembly files into machine code files.  
Developed in C, it functions as a two-pass assembler. The implementation of this program is based on a two-pass assembly approach.

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
    - Bit number 0 represents the least significant bit and bit number 31 represents the most significant bit.

#### Addresses
* The addresses range from 0 to (2^25-1), where each cell size is 1 byte.
*  A memory address is represented by 25 bits as an unsigned number.

#### Notes
* The assembler operates exclusively with positive and negative integers. Arithmetic follows the 2's complement method. 
* The assembler supports characters represented in ASCII code.



## How to Run
The project was coded and compiled using Ubuntu, but it may run on all Linux versions.  
  
Use makefile to compile the project:
```bash
>   make
```

After preparing assembly files **with an `.as` extension**, open *terminal* and pass file names as arguments as following:

```bash
>   assembler x.as y.as z.as
```



## The program's input and output

The assembler can take a maximum of 3 assembler files via the command line.  

It creates up to three new files based on each input file.  
These new files are named after the input file, using extensions like `.ent`, `.ext`, or `.ob`, depending on the content of the input file.

### Input File Structure

The input file should be an assembly file (.as extension) consisting of lines up to 80 characters. Each line can fall into one of these categories:

* Empty line: A line containing only spaces or tabs, or no characters at all.
* Comment line: Starts with the ';' character. These lines are ignored by the assembler.
* Directive line: Contains a single directive.
* Instruction line: Contains a single instruction.

### Output Files
* object file (`.obj`), which contains the machine code.
* externals file (`.ext`), with details of all the places (addresses) in the machine code where a label defined as external is coded.
* Entries file (`.ent`), with details on each label that is declared as an entry point (a symbol that appeared as an operand of the `.entry` directive,  
  and is characterized in the symbol table as an entry).

  

## Instructions

Each instruction in the system is comprised of an operation and operands, encoded as 32-bit machine code.  
The instructions occupy four consecutive memory bytes and are stored in little-endian format,
with the memory address referencing the lowest byte.

Each instruction contains an operation code (opcode) that identifies its function,  
 and some instructions have a secondary identification code (funct).

Instructions are categorized into three types: type R, type I, and type J.

### Instruction Table

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
* The instruction's names are case sensitive.
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
| opcode   | rs      | rt      | rd      | funct  | unused |
| :------- | :------ | :------ | :-------| :----- | :----  |
| 31 - 26  | 25 - 21 | 20 - 16 | 15 - 11 | 10 - 6 | 5 - 0  |

 Type Instructions instructions:
| opcode   | rs      | rt      | immed                     |
| :------- | :------ | :------ | :-------------------------|
| 31 - 26  | 25 - 21 | 20 - 16 | 15 - 0                    |

Type J instructions:
| opcode   | reg | address                                 |
| :------- | :-- | :---------------------------------------|
| 31 - 26  | 25  | 24 - 0                                  |



## Directives

### Structure of a directive
* Label (optional)
* The name of the directive
* Operands

### Directive Types
#### Directives `.dh` `.dw` `.db`    
  
The operands of these directives are integers (one or more) with exactly one comma between every two parameters.

These directives instruct the assembler to allocate space in the data image, where the values of the parameters will be stored, and to advance the data counter (DC), according to the number and size of the values.
- Parameters defined by the command statement `.db` occupy 1 byte.
- Parameters defined by the directive sentence `.dh` occupy 2 bytes.
- Parameters defined by the directive `.dw` occupy 4 bytes.

Notes:
- Each number must be of an appropriate size, which will not exceed the representation limits of the defined data type.
- If a label is defined in the instruction sentence, then this label receives the value of the data counter (before the promotion), and is inserted into the symbol table.

Example:
```  
.db 7,-57,18, +9
.dw 1200056
XYZ: .dh 0, -6431, 1700, 3, -1
```


#### Directive `.asciz`
The operand of this directive is a single valid *String*.
  
The string's characters are encoded according to the corresponding ASCII values
and are stored in the data image, each character using one byte.
  
To mark the string's end, '0/' is added.  
The assembler's data counter (DC) will be advanced according to the length of the string.  
If a label is included in the line, it gets the data counter's current value (before promotion) and is inserted into the data table.

Example:
```
STR: .asciz "Hello world"
```
#### Directive `.entry`
The parameter of this directive is the name of a label defined in the current file.  
The purpose of the `.entry` directive is to characterize this label in a way that allows assembly code found in other files to use it as an instruction operand.

Example:
   ```
   .entry HELLO
   ``` 

Directive: `.extern` 

The parameter of this directive is the name of a label that is not defined in the current file. Its purpose is to notify the assembler that the specified label is defined in another source file and that the current file's assembly code makes use of this label.

Example:
   ```
   .extern HELLO
   ```
  
Note: If a label is specified before `.extern`, the assembler ignores it.




## Demo

### Testing valid file
Inpute file: validTest.as
```bash
; file name: 'validTest.as'
; this file represents a valid input file to the assembler program.

	labelV1: addi $1, -2, $3
labelV2: add $10, $20, $30

labelV4: blt $5  , $15,  V3

	.entry labelV3
	.entry 	labelV4


blt $5   ,$24, labelV5

	.entry labelV5

jmp labelV5
jmp $5

labelV5: lh $4,14,$24

labelV3: 	move $2, $12

	.extern 	labelV11

la   labelV11

call labelV12

	.extern labelV12

labelV6: .db -128,-100	 ,-50,	 0,	 +10,	 +20 ,	30,	 127
	.db 1, 2

labelV7: .asciz "1he c2rrent 5tr+ng is v5lid!"
	.asciz 	"asciz directive without label definition"

labelV8: .dh -32768  , 200, +32767
	.dh 10  , 20

	.dw 100, 500
labelV9: .dw -2147483648,+2147483647, 11

labelV10: stop
```
Output files:  
validTest.ent
```
labelV4 0108
labelV5 0124
labelV3 0128
```
validTest.ext
```
labelV11 0132
labelV12 0136
```

validTest.ob
```
44 110
0100 FE FF 23 28
0104 40 F0 54 01
0108 00 00 AF 44
0112 0C 00 B8 44
0116 7C 00 00 78
0120 05 00 00 7A
0124 0E 00 98 5C
0128 40 60 40 04
0132 00 00 00 7C
0136 00 00 00 80
0140 00 00 00 FC
0144 80 9C CE 00 
0148 0A 14 1E 7F 
0152 01 02 31 68 
0156 65 20 63 32 
0160 72 72 65 6E 
0164 74 20 35 74 
0168 72 2B 6E 67 
0172 20 69 73 20 
0176 76 35 6C 69 
0180 64 21 00 61 
0184 73 63 69 7A 
0188 20 64 69 72 
0192 65 63 74 69 
0196 76 65 20 77 
0200 69 74 68 6F 
0204 75 74 20 6C 
0208 61 62 65 6C 
0212 20 64 65 66 
0216 69 6E 69 74 
0220 69 6F 6E 00 
0224 00 80 C8 00 
0228 FF 7F 0A 00 
0232 14 00 64 00 
0236 00 00 F4 01 
0240 00 00 00 00 
0244 00 80 FF FF 
0248 FF 7F 0B 00 
0252 00 00
```

### Testing all the errors the program has
Input file: errorTester.as
```bash
invalid.line.length.number.of.chars.so.far.is.49.111111111112222222222333333333344

		;**********instructions**********

		;invalid instruction name:
instruction 5,6,7
Add
		;invalid number of operands:
		
	;'R' arithmetic and logical instruction:
or
and $3
add $3, $19
sub $3, $19, $8, $30

	;'R' copy instructions:
mvlo
move $3
mvhi $3, $19, $8
	
	;'I' arithmetic and logical instructions:
ori
andi $9
addi $9, -45
	subi $9, -45, $8, $9
	
	;'I' Conditional branching instructions:
blt
bne $5 
beq $5, $24
	blt $5, $24, loop, $30
	
	;'I' Instructions for loading and keeping in memory:
lw
lb $9
sb $9, 34
lh $9, 34, $2, $4
	
	;'J' instructions - jump:
jmp
jmp label1, label2
jmp $6, $7

	;'J' instructions - la / call:
la
call label1, label2
	
	;stop instruction:
stop ab12

		; invalid operands:
		
	;'R' arithmetic and logical instruction:
add $3, symbol, $8
or  3, $19, $8
and $3, $19, $32
sub $3, $32, $8

	;'R' copy instructions:
move $3, symbol
mvhi 3, $5
mvlo $40, $5

	;'I' arithmetic and logical instructions:
ori 9, -45, $8
andi $9, 100000000, $8
addi $9, -45, label
nori $65, -45, $8

	;'I' Conditional branching instructions:
blt 5, $24, loop
bne $5, $44, loop
beq $5, $24, $3
bgt $5, $24, 5

	;'I' Instructions for loading and keeping in memory:
lw 9, 34, $2
lb $9, $34, $2
sb $9, 34, symbol
lh $9, 34, $32
sw $9, symbol, $20

	;'J' instructions - jump:
jmp 44

	;'J' instructions - la / call:
la $4
call 4


;		**********directives**********
		
		;invalid directive name:
.Asciz "abc"
.directive 9, 7, 2

		;invalid number of operands:
.db
.dw
.dh
.asciz

		;invalid operands:
.db $5
.db symbol
.db 128
.db -129

.dh $5
.dh symbol
.dh 32768
.dh -32769

.dw $5
.dw symbol
.dw 2147483648
.dw -2147483649

.asciz abc
.asciz "abc
.asciz "abc""
.asciz ""abc"
.asciz ""abc
.asciz abc""
.asciz "	"
.asciz """

		;**********commas**********
		
add ,$3, $19, $20
add ,	$3, $19, $20
mvhi $3, $19,
mvhi $3, $19	,
addi $9,,-45, $8
blt $5,	,$24, loop
lh $9 34, $2
lh $9,34	 $2

;		**********labels**********
add:
asciz:
label:
label :
1abel:
l able:
lab;l:
theLengthOfLabelLimitedTo31chars:

labelV1: .asciz "abcd"
labelV1: .asciz "abcd"
labelV1: add $1, $2, $3
.extern labelV1
```
Output errors:
```
Error! file 'errorTester .as' line 1: line is too long.
Error! file 'errorTester.as' line 6: Invalid instruction name.
Error! file 'errorTester.as' line 7: Invalid instruction name.
Error! file 'errorTester.as' line 11: Missing Operands.
Error! file 'errorTester.as' line 12: Incorrect number of registers. 'R' arithmetic and logical instructions should receive 3 registers.
Error! file 'errorTester.as' line 13: Incorrect number of registers. 'R' arithmetic and logical instructions should receive 3 registers.
Error! file 'errorTester.as' line 14: Incorrect number of registers. 'R' arithmetic and logical instructions should receive 3 registers.
Error! file 'errorTester.as' line 17: Missing Operands.
Error! file 'errorTester.as' line 18: Incorrect number of registers. 'R' copy instructions should receive 2 registers.
Error! file 'errorTester.as' line 19: Incorrect number of registers. 'R' copy instructions should receive 2 registers.
Error! file 'errorTester.as' line 22: Missing Operands.
Error! file 'errorTester.as' line 23: Incorrect number of operands. 'I' copy/loading/saving memory instructions should receive 3 operands.
Error! file 'errorTester.as' line 24: Incorrect number of operands. 'I' copy/loading/saving memory instructions should receive 3 operands.
Error! file 'errorTester.as' line 25: Incorrect number of operands. 'I' copy/loading/saving memory instructions should receive 3 operands.
Error! file 'errorTester.as' line 28: Missing Operands.
Error! file 'errorTester.as' line 29: Incorrect number of operands. 'I' Conditional branching instructions should receive 3 operands.
Error! file 'errorTester.as' line 30: Incorrect number of operands. 'I' Conditional branching instructions should receive 3 operands.
Error! file 'errorTester.as' line 31: Incorrect number of operands. 'I' Conditional branching instructions should receive 3 operands.
Error! file 'errorTester.as' line 34: Missing Operands.
Error! file 'errorTester .as' line 35: Incorrect number of operands. 'I' copy/Loading/saving memory instructions should receive 3 operands.
Error! file 'errorTester.as' line 36: Incorrect number of operands. 'I' copy/loading/saving memory instructions should receive 3 operands.
Error! file 'errorTester.as' line 37: Incorrect number of operands. 'I' copy/loading/saving memory instructions should receive 3 operands.
Error! file 'errorTester.as' line 40: Missing Operands.
Error! file 'errorTester.as' line 41: Incorrect number of operands, jump instruction should receive only one operand.
Error! file 'errorTester.as' line 42: Incorrect number of operands, jump instruction should receive only one operand.
Error! file 'errorTester.as' line 45: Missing Operands.
Error! file 'errorTester.as' line 46: Incorrect number of operands, jump instruction should receive only one operand.
Error! file 'errorTester.as' line 49: Excessive text after 'stop' instruction.
Error! file 'errorTester.as' line 54: Invalid register. register must start with '$' and represent a number between 0-31.
Error! file 'errorTester.as' line 55: Invalid register. register must start with '$' and represent a number between 0-31
Error! file 'errorTester.as' line 56: Invalid register. register must start with '$' and represent a number between 0-31.
Error! file 'errorTester.as' line 57: Invalid register. register must start with '$' and represent a number between 0-31.
Error! file 'errorTester.as' line 57: Invalid register. register must start with '$' and represent a number between 0-31.
Error! file 'errorTester.as' line 60: Invalid register. register must start with '$' and represent a number between 0-31.
Error! file 'errorTester.as' line 61: Invalid register. register must start with '$' and represent a number between 0-31.
Error! file 'errorTester.as' line 62: Invalid register. register must start with '$' and represent a number between 0-31.
Error! file 'errorTester.as' line 65: Invalid register. register must start with '$' and represent a number between 0-31.
Error! file 'errorTester .as' line 66: Invalid operand, Operand must be an integer in the range defined by the instruction/directive.
Error! file 'errorTester.as' line 67: Invalid register. register must start with '$' and represent a number between 0-31.
Error! file 'errorTester.as' line 68: Invalid register. register must start with '$' and represent a number between 0-31.
Error! file 'errorTester.as' line 71: Invalid register. register must start with '$' and represent a number between 0-31.
Error! file 'errorTester.as' line 72: Invalid register. register must start with '$' and represent a number between 0-31.
Error! file 'errorTester .as' line 73: Invalid label, Syntactic error.
Error! file 'errorTester .as' line 74: Invalid label, Syntactic error.
Error! file 'errorTester.as' line 77: Invalid register. register must start with '$' and represent a number between 0-31.
Error! file 'errorTester .as' line 78: Invalid operand, Operand must be an integer in the range defined by the instruction/directive.
Error! file 'errorTester.as' line 79: Invalid register. register must start with '$' and represent a number between 0-31.
Error! file 'errorTester.as' line 80: Invalid register. register must start with '$' and represent a number between 0-31.
Error! file 'errorTester .as' line 81: Invalid operand, Operand must be an integer in the range defined by the instruction/directive.
Error! file 'errorTester .as' line 84: Invalid label, Syntactic error.
Error! file 'errorTester .as' line 87: Invalid label, Syntactic error.
Error! file 'errorTester .as' line 88: Invalid label, Syntactic error.
Error! file 'errorTester.as' line 94: Unrecognized directive word.
Error! file 'errorTester.as' line 95: unrecognized directive word.
Error! file 'errorTester.as' line 98: Missing Operands.
Error! file 'errorTester.as' line 99: Missing Operands.
Error! file 'errorTester.as' line 100: Missing Operands.
Error! file 'errorTester.as' line 101: Missing Operands.
Error! file 'errorTester .as' line 104: Invalid operand, Operand must be an integer in the range defined by the instruction/directive.
Error! file 'errorTester .as' line 105: Invalid operand, Operand must be an integer in the range defined by the instruction/directive.
Error! file 'errorTester .as' line 106: Invalid operand, Operand must be an integer in the range defined by the instruction/directive.
Error! file 'errorTester .as' line 107: Invalid operand, Operand must be an integer in the range defined by the instruction/directive.
Error! file 'errorTester .as' line 109: Invalid operand, Operand must be an integer in the range defined by the instruction/directive.
Error! file 'errorTester .as' line 112: Invalid operand, Operand must be an integer in the range defined by the instruction/directive.
Error! file 'errorTester .as' line 114: Invalid operand, Operand must be an integer in the range defined by the instruction/directive.
Error! file 'errorTester .as' line 115: Invalid operand, Operand must be an integer in the range defined by the instruction/directive.
Error! file 'errorTester .as' line 119: The string is not bounded by quotes.
Epror! file 'errorTester as' line 120: The string is not bounded by quotes.
Error! file 'errorTester as' Line 121: The string is not bounded by quotes.
Epror! file 'errorTester as' line 122: The string is not bounded by quotes.
Error! file 'errorTester .as' line 123: The string is not bounded by quotes.
Error! file 'errorTester .as' line 124: The string is not bounded by quotes.
Error! file 'errorTester .as' line 125: String contain char that cannot be printed.
Error! file 'errorTester .as' line 126: The string is not bounded by quotes.
Error! file 'errorTester .as' line 130: A comma appears before the first variable or after the last variable.
Error! file 'errorTester .as' line 131: A comma appears before the first variable or after the last variable.
Error! file 'errorTester .as' line 132: A comma appears before the first variable or after the last variable.
Error! file 'errorTester .as' line 133: A comma appears before the first variable or after the last variable.
Error! file 'errorTester .as' line 134: multiple commas.
Error! file 'errorTester.as' line 135: multiple commas.
Epror! file 'errorTester .as' Line 136: Missing comma.
Error! file 'errorTester .as' line 137: Missing comma.
Error! file 'errorTester .as' Line 140: Invalid label, the Label name is a reserved instruction/directive word.
Error! file 'errorTester .as' line 141: Invalid label, the Label name is a reserved instruction/directive word.
Error! file 'errorTester .as' line 142: Missing instruction/directive after label definition.
Eppop! file 'errorTester .as' Line 143: Invalid Label, Syntactic error.
Eppop! file 'errorTester .as' Line 144: Invalid Label, Syntactic error.
Eppop! file 'errorTester .as' Line 145: Invalid label, Syntactic error.
Error! file 'errorTester .as' Line 146: Invalid Label, Syntactic error.
Error! file 'errorTester as' Line 147: The Label name is Longer than 31 characters.
Erpor! file 'errorTester .as' line 150: Label is already defined.
Erpor! file 'errorTester .as' line 151: Label is already defined.
Erpor! file 'errorTester .as' line 152: Label is already defined.
```



## Author
#### *Nick Savchenko*
