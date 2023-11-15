
#ifndef UNTITLED_FILES_H
#define UNTITLED_FILES_H

#include <stdio.h>
#include "structs.h"


/*
   This  file provides functions essential for handling and processing the given '.asm' input files.
   It contains  functions responsible for opening, processing, and validating input files according to a specific format.
*/


/*
    Function: fileIsValid
    Description:
        Processes the given file and checks if it's valid by performing the first and second passes of the assembly.
    Parameters:
        - fileName: Name of the file to be processed.
        - codeImage: Pointer to the code image table.
        - dataImage: Pointer to the data image table.
        - attrTab: Pointer to the attributes table.
        - ICF: Pointer to the Instruction Counter Final value.
        - DCF: Pointer to the Data Counter Final value.
    Returns:
        - state: Returns the state (VALID/INVALID) indicating whether the file is valid or not.
    Notes:
        - Manages the opening, processing, and validation of the assembly file.
        - Invokes the first and second passes of the assembly process.
*/
state fileIsValid(const char *fileName, codeTable *codeImage, dataTable *dataImage, attributesTable *attrTab, long *ICF, long *DCF);


/*
    Function: openFile
    Description:
        Opens the required file for reading/writing and handles potential errors in the file opening process.
    Parameters:
        - fileDescriptor: Pointer to the file descriptor.
        - fileName: Name of the file to be opened.
        - mode: Mode for opening the file (read/write).
    Returns:
        - state: Returns the state (VALID/INVALID) indicating the success of file opening.
    Notes:
        - Manages file opening according to the specified mode (read/write).
        - If the file opening fails, prints an error.
*/
state openFile(FILE **fileDescriptor, const char *fileName, char *mode);


/*
    Function: getFileFullName
    Description: Generates the full name of the file, including the extension.
    Parameters:
        - fileName: Name of the file without the extension.
        - fileExtension: File extension to be added to the file name.
    Returns:
        - char*: Returns the full file name including the extension.
    Notes:
        - Allocates memory for the full file name and returns the generated name.
*/
char * getFileFullName(char *fileName, char *fileExtension);


/*
    Function: updateValues
    Description: Updates the final values of the Instruction Counter, Data Counter, symbol table, and data table
                 based on the first pass processing.
    Parameters:
        - ICF: Pointer to the Instruction Counter Final value.
        - DCF: Pointer to the Data Counter Final value.
        - IC: Final value of the Instruction Counter.
        - DC: Final value of the Data Counter.
        - symTab: Symbol table containing information about symbols.
        - dataTab: Data table containing information about data elements.
    Returns:
        - void
    Notes:
        - Updates the dataType of each symbol in the symbol table that characterized as 'data'.
        - Updates the addresses of all data entries in the data image table by incrementing each address with ICF value.
        - Preserves the final values of IC and DC
*/
void updateValues(long *ICF, long *DCF, long IC, long DC, symbolTable symTab, dataTable dataTab);


/*
    Function: processFile
    Description: Processes the given file, handling the first and second passes, and updating relevant values.
    Parameters:
        - codeImage: Pointer to the code image table.
        - dataImage: Pointer to the data image table.
        - attrTab: Pointer to the attributes table.
        - ICF: Pointer to the Instruction Counter Final value.
        - DCF: Pointer to the Data Counter Final value.
        - fd: Pointer to the file descriptor.
        - line: Pointer to the structure representing a line of code.
    Returns:
        - state: Returns the state (VALID/INVALID) indicating the success of file processing.
    Notes:
        - Invokes first and second passes, updates values, and manages file processing.
*/
state processFile(codeTable *codeImage, dataTable *dataImage, attributesTable *attrTab, long *ICF, long *DCF, FILE *fd, newLine *line);


#endif