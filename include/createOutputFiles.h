#ifndef UNTITLED_CREATEOUTPUTFILES_H
#define UNTITLED_CREATEOUTPUTFILES_H

#include <stdio.h>
#include "structs.h"


/* This file defines functions responsible for exporting assembly-related data to object ,external and entry files. */


/*
    Function: createOutputFiles
    Description: Creates output files based on provided data and frees the memory that was allocated for this data.
    Parameters:
        - fileName: Name of the file.
        - codeImage: Table containing code image data.
        - dataImage: Table containing data image data.
        - attributesTab: Table containing attributes data.
        - ICF: Instruction Counter Final value.
        - DCF: Data Counter Final value.
    Returns:
        - void
*/
void createOutputFiles(char *fileName, codeTable codeImage, dataTable dataImage, attributesTable attributesTab, long ICF, long DCF);


/*
    Function: createObjectFile
    Description: Creates an object file based on code and data images and their sizes, using the provided file name.
    Parameters:
        - fileName: Name of the file to be created.
        - codeImage: Table containing code image data.
        - dataImage: Table containing data image data.
        - ICF: Instruction Counter Final value.
        - DCF: Data Counter Final value.
    Returns:
        - void
    File Structure:
        - The first line indicates the size of the code and data images, represented in decimal format.
        - The rest of the lines represent the memory image. Each line comprises 5 fields:
              Field 1: Memory address (in 4-digit decimal format),
              Fields 2-5: content of 4 bytes stored in memory after this address, arranged in ascending order from left to right.
        - The address is displayed in decimal format, while each byte's content is shown in two-digit hexadecimal format.
        - The final row may contain fewer than 4 bytes.
        - The code image is displayed first in the file.
        - The file name is generated using the input file name (without the extension) and appending the '.ob' extension.
*/
void createObjectFile(char *fileName, codeTable codeImage, dataTable dataImage, long ICF, long DCF);


/*
    Function: createExtAndEntFiles
    Description: Creates external and entry files based on the attributes table and the provided file name.
    Parameters:
        - fileName: Name of the file to be created.
        - attributesTab: Table containing attributes data.
    Returns:
        - void
*/
void createExtAndEntFiles(char *fileName, attributesTable attributesTab);


/*
    Function: exportToExtAndEntFiles
    Description: Exports symbols defined as external and internal to the corresponding external and entry files.
    Parameters:
        - attributesTab: Table containing attributes data.
        - externFileDescriptor: Pointer to the external file descriptor.
        - entryFileDescriptor: Pointer to the entry file descriptor.
    Returns:
        - void
    Notes:
        - Exports the symbols along with their corresponding addresses.
        - Each line in the file consists of a single label and its associated address, presented as 4-digit decimal number.
        - The filename for each file consists of the input filename (without the extension),
          followed by either the 'ext' extension (for externals file) or the 'ent' extension (for entries file).
*/
void exportToExtAndEntFiles(attributesTable attributesTab, FILE *externFileDescriptor, FILE  *entryFileDescriptor);


/*
    Function: exportCodeImage
    Description: Exports the code image into object file in a specific format based on the dataType of instruction.
    Parameters:
        - codeImage: Table containing code image data.
        - fd: File descriptor for the output file.
    Returns:
        - void
*/
void exportCodeImage(codeTable codeImage, FILE *fd);


/*
    Function: exportDataImage
    Description: Exports the data image to the specified file descriptor, starting from a specified address.
    Parameters:
        - dataImage: Table containing data image data.
        - fileDescriptor: File descriptor for the output file.
        - startingAddress: Starting address for the data export.
    Returns:
        - void
*/
void exportDataImage(dataTable dataImage, FILE *fileDescriptor, long startingAddress);


/*
    Function: exportDataVariable
    Description: Exports a single variable from the data entry to the specified file descriptor.
    Parameters:
        - currentEntry: Current data entry containing variables.
        - currentAddress: Pointer to the current memory address.
        - currentIndex: Index of the current variable.
        - fileDescriptor: File descriptor for the output file.
        - totalPrintedBytes: Pointer to the total number of bytes printed.
    Returns:
        - void
*/
void exportDataVariable(dataTable currentEntry, long *currentAddress, int currentIndex, FILE *fileDescriptor, long *totalPrintedBytes);


/*
    Function: exportDataEntry
    Description: Exports the data of a single data image entry to the specified file descriptor.
    Parameters:
        - currentEntry: Current data entry to be exported.
        - currentAddress: Pointer to the current memory address.
        - totalPrintedBytes: Pointer to the total number of bytes printed.
        - fileDescriptor: File descriptor for the output file.
    Returns:
        - void
*/
void exportDataEntry(dataTable currentEntry, long *currentAddress, long *totalPrintedBytes, FILE *fileDescriptor);


/*
    Function: exportDataByteByType
    Description: Exports a single byte of data based on the type of data entry to the specified file descriptor.
    Parameters:
        - currentEntry: Current data entry.
        - printedVariableBytes: Number of bytes printed for the current variable.
        - currentIndex: Index of the current variable.
        - fileDescriptor: File descriptor for the output file.
    Returns:
        - void
*/
void exportDataByteByType(dataTable currentEntry, int printedVariableBytes, int currentIndex, FILE *fileDescriptor);


/*
    Function: exportDWByte
    Description: Exports bytes of '.dw' directive variables to the specified file descriptor.
    Parameters:
        - currentVariable: Pointer to the current variable.
        - printedVariableBytes: Number of bytes printed for the current variable.
        - fileDescriptor: File descriptor for the output file.
    Returns:
        - void
*/
void exportDWByte(void* currentVariable, int printedVariableBytes, FILE* fileDescriptor);


/*
    Function: exportDHByte
    Description: Exports bytes of '.dh' directive variables to the specified file descriptor, based on the number of bytes already printed.
    Parameters:
        - currentVariable: Pointer to the current variable.
        - printedVariableBytes: Number of bytes already printed for the current variable.
        - fileDescriptor: File descriptor for the output file.
    Returns:
        - void
*/
void exportDHByte(void* currentVariable, int printedVariableBytes, FILE* fileDescriptor);


#endif