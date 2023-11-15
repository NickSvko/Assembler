
#ifndef UNTITLED_SECONDPASS_H
#define UNTITLED_SECONDPASS_H

#include "structs.h"


/* This file contains functions responsible for performing the second of the two passes on the input file */


/*
    Function: secondPass
    Description: Performs a second pass on a single input file, checking the validity of each line.
    Parameters:
        - line: Pointer to the structure representing the current line.
        - symbolTab: Pointer to the symbol table structure.
        - codeImage: Pointer to the code image table structure.
        - attributesTab: Pointer to the attributes table structure.
        - fd: Pointer to the input file stream.
    Returns:
        - The state of the process. state variable: VALID / INVALID.
    Notes:
        - This function processes each line from the input file, performing a second pass.
        - It checks the validity of each line and performs necessary operations based on the line type (directive/instruction).
        - Updates the attributes table, symbol table, and code image table as required during the second pass.
        - Returns the state of the overall process.
          Returns VALID if all line are processed without errors, otherwise, returns INVALID.
*/
state secondPass(newLine *line, symbolTable symbolTab, codeTable codeImage, attributesTable *attributesTab, FILE *fd);


/*
    Function: lineSecondPass
    Description: Performs a second pass on a single line from the input file.
    Parameters:
        - line: Pointer to the structure representing the current line.
        - symbolTab: Pointer to the symbol table structure.
        - codeImage: Pointer to the code image table structure.
        - attributesTab: Pointer to the attributes table structure.
    Returns:
        - The state of the line's validation. state variable: VALID / INVALID.
        - The state of the line's validation. state variable: VALID / INVALID.
    Notes:
        - This function processes a single line from the input file during the second pass.
        - It checks the type of line (directive/instruction) and performs appropriate actions based on the line type.
        - Updates the symbol table, code image table, and attributes table as required for the line.
        - Returns the state of the line's validation.
          Returns VALID if the line is processed without errors; otherwise, returns INVALID.
*/
state lineSecondPass(newLine *line, symbolTable symbolTab, codeTable codeImage, attributesTable *attributesTab);


#endif
