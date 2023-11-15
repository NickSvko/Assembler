#ifndef UNTITLED_INSTRUCTIONS_H
#define UNTITLED_INSTRUCTIONS_H

#include "structs.h"


/*
   This file Provides tools for parsing, validating, and processing the available assembly instructions.
   It enables efficient handling of different instruction types and their respective syntax rules.
*/


/*
    Function: instructionWordState
    Description: returns if the current word is a valid instruction word and updates the instruction token accordingly.
    Parameters:
        - line: Pointer to the current line being processed.
        - instructionToken: Pointer to the instruction token structure.
        - index: Pointer to the index in the line content.
    Returns:
        - The state of the instruction word. state variable: VALID \ INVALID.
    Notes:
        - Validates the syntax of the instruction word and searches for a match among reserved instructions.
        - if the given word invalid, adds the proper error to the line object.
*/
state instructionWordState(newLine *line, instructionWord *instructionToken, int *index);


/*
    Function: searchInstruction
    Description: Searches for a match between the current instruction and the available instructions.
                 If a match was found, updating the instruction token.
    Parameters:
        - instructionToken: Pointer to the instruction token structure.
    Returns:
        - The validation state of the instruction. state variable: VALID \ INVALID.
*/
state searchInstruction(instructionWord *instructionToken);


/*
    Function: instructionLineState
    Description: Checks if the instruction line is valid, verifying syntax and operands.
    Parameters:
        - line: Pointer to the current line being processed.
        - instructionToken: The instruction token structure.
        - contentIndex: Index in the line content.
    Returns:
        - The instruction line validation state. state variable: VALID \ INVALID.
    Notes:
        - The validation process is carry out according to the instruction token.
*/
state instructionLineState(newLine *line, instructionWord instructionToken, int contentIndex);


/*
    Function: checkJOperandsSyntax
    Description: Checks the syntax of J-dataType instruction.
    Parameters:
        - line: Pointer to the current line being processed.
        - opcode: Unsigned integer representing the opcode.
        - symbol: Pointer to a character symbol.
        - index: Pointer to the index in the line content.
        - numOfScannedOperands: Pointer to the number of scanned operands.
    Returns:
        - void
*/
void checkJOperandsSyntax(newLine *line, unsigned int opcode, char *symbol, int *index, int *numOfScannedOperands);


/*
    Function: checkIOperandsSyntax
    Description: Checks the syntax of I-dataType instruction.
    Parameters:
        - line: Pointer to the current line being processed.
        - opcode: Unsigned integer representing the opcode.
        - symbol: Pointer to a character symbol.
        - index: Pointer to the index in the line content.
        - numOfScannedOperands: Pointer to the number of scanned operands.
    Returns:
        - void
*/
void checkIOperandsSyntax(newLine *line, unsigned int opcode, char *symbol, int *index, int *numOfScannedOperands);


/*
    Function: checkOperandByType
    Description: Checks the validation of an instruction operand based on the instruction dataType.
    Parameters:
        - line: Pointer to the current line being processed.
        - opcode: Unsigned integer representing the opcode.
        - symbol: Pointer to a character symbol.
        - index: Pointer to the index in the line content.
        - numOfScannedOperands: Pointer to the number of scanned operands.
    Returns:
        - void
*/
void checkOperandByType(newLine *line, unsigned int opcode, char *symbol, int *index, int *numOfScannedOperands);


/*
    Function: checkInstructionSyntax
    Description: Checks the syntax and validation of an instruction line.
    Parameters:
        - line: Pointer to the current line being processed.
        - opcode: Unsigned integer representing the opcode.
        - contentIndex: Index in the line content.
    Returns:
        - void
    Notes:
        - The validation of the syntax and operands of an instruction line based on the opcode and the contentIndex.
*/
void checkInstructionSyntax(newLine *line, unsigned int opcode, int contentIndex);


/*
    Function: getInstruction
    Description: Retrieves the instruction from the content at a specific index and updates the instructionToken.
    Parameters:
        - content: Pointer to the content being processed.
        - contentIndex: Pointer to the index in the content.
        - instructionToken: Pointer to the instruction word structure.
    Returns:
        - void
*/
void getInstruction(const char *content, int *contentIndex, instructionWord *instructionToken);


/*
    Function: getAddress
    Description: Calculates and updates the requested dataType and returns its validation state.
    Parameters:
        - line: Pointer to the current line being processed.
        - instructionAddress: Long integer representing the instruction address.
        - label: Pointer to the symbol table entry representing the label.
        - type: Instruction type (I, J, R).
        - address: Pointer to the address variable to be updated.
    Returns:
        - The state of the address. state variable: VALID / INVALID.
    Notes:
        - The calculation based on  the instruction dataType.
*/
state getAddress(newLine *line, long instructionAddress, symbolTable label, instructionType type, long *address);


/*
    Function: addressState
    Description: Determines if the given address is valid.
    Parameters:
        - line: Pointer to the current line being processed.
        - label: Pointer to the symbol table entry representing the label.
        - type: Instruction type (I, J, R).
        - address: Long integer representing the address.
    Returns:
        - The State of the address validation. state variable: VALID / INVALID.
    Notes:
        - The validation based on the instruction type and label.
*/
state addressState(newLine *line, symbolTable label, instructionType type, long address);


/*
    Function: processInstruction
    Description: Processes the instruction and updates symbol and code tables accordingly.
    Parameters:
        - line: Pointer to the current line being processed.
        - index: Pointer to the index in the content.
        - labelSet: Boolean indicating if a label is set for the instruction.
        - label: Pointer to the label string.
        - symTab: Pointer to the symbol table.
        - cImage: Pointer to the code image table.
        - IC: Pointer to the instruction counter.
    Returns:
        - void
*/
void processInstruction(newLine *line, int *index, bool labelSet , char *label, symbolTable *symTab, codeTable *cImage, long *IC);


/*
    Function: instructionWithLabelOperand
    Description: Determines if the instruction has a label operand.
    Parameters:
        - line: Pointer to the current line being processed.
        - index: Index in the content.
        - opcode: Unsigned integer representing the instruction opcode.
    Returns:
        - If the instruction has a label operand. bool variable: TRUE / FALSE.
    Notes:
        - Determines if the instruction has a label operand based on the provided opcode.
*/
bool instructionWithLabelOperand(newLine *line, int index, unsigned int opcode);


/*
    Function: getReservedInstructions
    Description: Retrieves the list of reserved instructions and updates the number of instructions.
    Parameters:
        - numberOfInstructions: Pointer to an integer variable to store the number of instructions.
    Returns:
        - Pointer to the array of reserved instruction words. pointer to instructionWord variable.
    Notes:
        - Each instruction contains information about its name, opcode, funct and dataType.
*/
instructionWord *getReservedInstructions(int *numberOfInstructions);


#endif