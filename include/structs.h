
#ifndef UNTITLED_STRUCTS_H
#define UNTITLED_STRUCTS_H

#include "globals.h"

/*
    This file defines and organizes structures and types used in the assembler.
    It covers symbols, instructions, and data encountered while parsing code and input.
    It also defines how these elements are stored and managed.
 */


/*
    Enum: imageType
    Description: Represents the available symbol types (code, data, entry, external).
 */
typedef enum symbolType {code, data, entry, external} imageType;


/*
    Enum: directiveType
    Description: Represents the available directive types (DH, DW, DB, ASCIZ, ENTRY, EXTERN, NONE).
 */
typedef enum {DH, DW, DB, ASCIZ, ENTRY, EXTERN, NONE} directiveType;


/*
   Enum: instructionType
   Description: Represents the categorization of instruction types (R , I , J).
*/
typedef enum instructionType {R,I,J} instructionType;


/*
   Structure: instructionWord
   Description: Represents the definition of an instruction word.
   Fields:
     - name: Represents the name of the instruction.
     - opcode: Represents the opcode of the instruction.
     - funct: Represents the function code of the instruction.
     - type: Represents the type of instruction (R for Register, I for Immediate, or J for Jump).
     - address: Represents the data type or address associated with the instruction.
   Notes:
     - Encapsulates: name, opcode, function code, type, and associated data/address.
*/
typedef struct instructionWord {
    char name[maxInstructionLength];
    unsigned int opcode;
    unsigned int funct;
    instructionType type;
    long address;
} instructionWord;


/*
   Structure: directiveWord
   Description: Represents the definition of a directive word.
   Fields:
     - name: Represents the name of the directive.
     - type: Represents the type of directive (DH, DW, DB, ASCIZ, ENTRY, EXTERN, or NONE).
   Notes:
     - Stores the name and type associated with the directive.
*/
typedef struct directiveWord {
    char name[maxDirectiveName];
    directiveType type;
} directiveWord;


/*
   Structure: typeRInstruction
   Description: Represents the binary structure of an R-type instructionWord.
   Fields:
     - unused: Represents unused bits (0 to 5).
     - funct: Represents the funct bits (6 to 10).
     - rd: Represents the third register or unused bits (11 to 15).
     - rt: Represents the second register bits (16 to 20).
     - rs: Represents the first register bits (21 to 25).
     - opcode: Represents the opcode bits (26 to 31).
   Notes:
     - Encodes the specific fields for an R-type instruction in a binary representation.
*/
typedef struct typeRInstruction
{
    unsigned int unused: 6;
    unsigned int funct: 5;
    unsigned int rd: 5;
    unsigned int rt: 5;
    unsigned int rs: 5;
    unsigned int opcode: 6;
} typeRInstruction;


/*
   Structure: typeIInstruction
   Description: Represents the binary structure of an I-type instruction.
   Fields:
     - immed: Represents the fixed/immediate bits (0 to 15).
     - rt: Represents the second register bits (16 to 20).
     - rs: Represents the first register bits (21 to 25).
     - opcode: Represents the opcode bits (26 to 31).
   Notes:
     - Encodes the specific fields for an I-type instruction in a binary representation.
*/
typedef struct typeIInstruction
{
    signed int immed: 16;
    unsigned int rt: 5;
    unsigned int rs: 5;
    unsigned int opcode: 6;
} typeIInstruction;


/*
   Structure: typeJInstruction
   Description: Represents the binary structure of a J-type instruction.
   Fields:
     - address: Represents the address bits (0 to 24).
     - reg: Represents the register bit (bit 25).
     - opcode: Represents the opcode bits (26 to 31).
   Notes:
     - Encodes the specific fields for a J-type instruction in a binary representation.
*/
typedef struct typeJInstruction
{
    unsigned int address: 25;
    unsigned int reg: 1;
    unsigned int opcode: 6;
} typeJInstruction;


/*
   Union: codeType
   Description: Represents the bit fields defined by instruction data type.
   Fields:
     - typeR: Structure for R-type instructions.
     - typeI: Structure for I-type instructions.
     - typeJ: Structure for J-type instructions.
   Notes:
     - The union stores different structures for R, I, and J-type instructions,
       allowing access to the specific bit fields based on the instruction type.
*/
typedef union codeType {
    typeRInstruction typeR;
    typeIInstruction typeI;
    typeJInstruction typeJ;
} codeType;


/* Pointer to a structure representing an entry in the symbol table */
typedef struct symbolTableEntry* symbolTable;

/*
   Struct: symbolTableEntry
   Description: Represents an entry in the symbol table,
                Preserves symbols and their numerical values encountered while parsing the source file.
   Fields:
     - name: The name of the symbol.
     - value: The numerical value associated with the symbol.
     - type: The image type associated with the symbol (code, data, entry, external).
     - isEntry: Boolean indicating if the symbol is marked as an 'entry'.
     - isExternal: Boolean indicating if the symbol is marked as 'external'.
     - next: Pointer to the next entry in the symbol table.
*/
typedef struct symbolTableEntry
{
    char *name;
    long value;
    imageType type;
    bool isEntry;
    bool isExternal;
    struct symbolTableEntry *next;
} symbolTableEntry;

/* Pointer to a structure representing an entry in the code image table */
typedef struct codeImageEntry *codeTable;

/*
   Struct: codeImageEntry
   Description: Represents an entry in the code image table,
                Preserves the encodings of machine instructions encountered while parsing the source file.
   Fields:
     - lineNumber: The line number in the source code where the instruction is located.
     - address: The memory address where the instruction is stored.
     - type: The type of instruction: R, I, or J.
     - data: Pointer to the machine code representation of the instruction (32-bit).
     - next: Pointer to the next entry in the code image.
*/
typedef struct codeImageEntry {
    long lineNumber;
    int address;
    instructionType type;
    codeType *data;
    struct codeImageEntry *next;
} codeImageEntry;


/* Pointer to a structure representing an entry in the data image table */
typedef struct dataImageEntry *dataTable;

/*
   Struct: dataImageEntry
   Description: Represents an entry in the data image table,
                maintains the encoding of data extracted from the source file (command lines of type: .db/.dw/.dh).
     - address: The address where the data is stored.
     - dataType: The directive type associated with the data (DB, DH, DW, ASCIZ, ENTRY, EXTERN, NONE).
     - variableSize: The size of a single data variable in bytes.
     - numOfVariables: The number of variables stored in the data entry.
     - data: Pointer to the stored data.
     - dataSize: The size of the current data in bytes.
     - next: Pointer to the next entry in the data image.
*/
typedef struct dataImageEntry
{
    long address;
    directiveType dataType;
	int variableSize;
    unsigned int numOfVariables;
    void *data;
    int dataSize;
    struct dataImageEntry *next;
} dataImageEntry;


/* Pointer to a structure representing an entry in the attributes table */
typedef struct attributesTableEntry *attributesTable;

/*
   Struct: attributesTableEntry
   Description: Represents an entry in the attributes table, storing label attributes and associated data.
   Fields:
     - type: The image type representing the attribute (entry or external).
     - name: Pointer to the label's name.
     - address: The address associated with the label (if it's an entry) or instruction (if it's external).
     - next: Pointer to the next entry in the attributes table.
*/
typedef struct attributesTableEntry
{
    imageType type;
    char *name;
    long address;
    struct attributesTableEntry *next;
} attributesTableEntry;

/*
   Struct: newLine
   Description: Represents a line read from the source file during the assembly process.
   Fields:
     - number: Line number in the source file.
     - sourceFileName: Name of the file from which the line was extracted.
     - content: Content of the line read from the source file.
     - error: Indicates any encountered error during processing of the line.
   Notes:
     - Encapsulates the details of a line read from the source file during assembly.
     - Stores essential information like line number, source file name, content, and error status.
*/
typedef struct line
{
    long number;
    const char *sourceFileName;
    const char *content;
    char *error;
} newLine;


#endif