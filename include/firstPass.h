#ifndef UNTITLED_FIRSTPASS_H
#define UNTITLED_FIRSTPASS_H

#include <stdio.h>
#include "structs.h"

/*
    This  file contains  functions related to the first pass of the two-passes assembly program.
    It provides functions that responsible for managing directives, instructions,
    labels, and line handling during the initial parsing and validation stages of the input file.
 */


/*
    Function: firstPass
    Description: Executes the first pass on the entire input file, parsing and validating each line.
    Parameters:
        - line: Pointer to the structure representing a line of code.
        - IC: Pointer to the Instruction Counter.
        - DC: Pointer to the Data Counter.
        - symTab: Pointer to the symbol table.
        - cImage: Pointer to the code image table.
        - dImage: Pointer to the data image table.
        - fd: Pointer to the input file descriptor.
    Returns:
        - Returns the validation state of the input file after the first pass. state variable: VALID / INVALID.
    Notes:
        - Uses lineFirstPass() function for each line's processing.
        - Handles directives, instructions, labels, and line errors during the assembly's first pass.
*/
state firstPass(newLine *line, long *IC, long *DC, symbolTable *symTab, codeTable *cImage, dataTable *dImage, FILE *fd);


/*
    Function: lineFirstPass
    Description: Executes the first pass on a single line from the input file.
    Parameters:
        - line: Pointer to the structure representing a line of code.
        - IC: Pointer to the Instruction Counter.
        - DC: Pointer to the Data Counter.
        - symTab: Pointer to the symbol table.
        - cImage: Pointer to the code image table.
        - dImage: Pointer to the data image table.
    Returns:
        - The validation state of the line, after the first pass.  state variable: VALID / INVALID.
    Notes:
        - Handles line-specific processing including label settings, directives, and instructions.
*/
state lineFirstPass(newLine *line, long *IC, long *DC, symbolTable *symTab, codeTable *cImage, dataTable *dImage);


#endif