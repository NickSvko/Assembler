
#ifndef UNTITLED_BINARY_H
#define UNTITLED_BINARY_H

#include "structs.h"


/*
   This  file contains functions related to the binary encoding of instructions in the assembler program.
   It facilitates the completion of binary encoding based on the instruction type,
   and presence of label operands in the source code.
 */


/*
    Function: checkLineBinaryEncoding
    Description: Checks the current line for instructions with label operands and completes the binary encoding if required.
    Parameters:
        - line: Pointer to the structure representing the current line.
        - symTable: Symbol table containing information about labels and symbols encountered.
        - cTable: Code table containing encoded instructions.
        - instructionToken: Pointer to the structure representing the current instruction.
        - label: Pointer to the symbol table entry of the label found, if present.
    Notes:
        - This function checks if the current line contains instructions with label operands ('I' or 'J' types).
        - If such instructions are found, it attempts to extract and process the associated label information.
        - Upon successful retrieval of label information from the symbol table,
          it initiates the completion of binary encoding.
*/
void checkLineBinaryEncoding(newLine *line, symbolTable symTable, codeTable cTable, instructionWord *instructionToken, symbolTable *label);


/*
    Function: checkLineBinaryEncoding
    Description: Manages the completion of binary encoding based on the instruction type and presence of label operands in the source code.
    Parameters:
        - line: Pointer to the structure representing the current line.
        - symTable: Symbol table containing information about labels and symbols encountered.
        - cTable: Code table containing encoded instructions.
        - instructionToken: Pointer to the structure representing the current instruction.
        - label: Pointer to the symbol table entry of the label found, if present.
    Notes:
        - This function scans the current line to identify instructions with label operands ('I' or 'J' types).
        - Upon encountering such instructions, it attempts to retrieve associated label information from the symbol table.
        - If the label is found in the symbol table,
          indicating its validity, this function manages the completion of binary encoding for the line.
*/
void completeLineBinaryEncoding(symbolTable label, codeTable table, instructionWord *instructionToken, newLine *line);


#endif