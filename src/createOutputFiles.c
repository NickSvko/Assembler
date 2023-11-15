
/* For 'free' function */
#include <stdlib.h>
/* For 'getFileFullName', 'openFile' functions and <stdio.h> functions */
#include "../include/files.h"
/* For 'freeTables' function */
#include "../include/tables.h"


void exportDWByte(void* currentVariable, int printedVariableBytes, FILE* fileDescriptor)
{
    /* If we need to print the 0-7 bits of the variable */
    if(printedVariableBytes == 0)
        fprintf(fileDescriptor,"%02X ", ((*(int *)currentVariable)) & 0xFF);
        /* If we need to print the 8-15 bits of the variable */
    else if(printedVariableBytes == 1)
        fprintf(fileDescriptor,"%02X ", ((*(int *)currentVariable) >> 8) & 0xFF);
        /* If we need to print the 16-23 bits of the variable */
    else if(printedVariableBytes == 2)
        fprintf(fileDescriptor,"%02X ", ((*( int *)currentVariable) >> 16) & 0xFF);
        /* If we need to print the 23-31 bits of the variable */
    else if(printedVariableBytes == 3)
        fprintf(fileDescriptor,"%02X ", ((*(int *)currentVariable) >> 24) & 0xFF);
}


void exportDHByte(void* currentVariable, int printedVariableBytes, FILE* fileDescriptor)
{
    /* If we need to print the 0-7 bits of the variable */
    if(printedVariableBytes == 0)
        fprintf(fileDescriptor,"%02X ", (*(short *)currentVariable) & 0xFF);

    /* If we need to print the 8-15 bits of the variable */
    if(printedVariableBytes == 1)
        fprintf(fileDescriptor,"%02X ", ((*(short *)currentVariable) >> 8) & 0xFF);
}


void exportDataByteByType(dataTable currentEntry, int printedVariableBytes, int currentIndex, FILE *fileDescriptor)
{
    /* This is a pointer referencing the current variable within the data to be exported */
    void *currentVariable;

    /* The memory location pointed to by 'currentVariable' varies according to the type of directive. */
    if(currentEntry->dataType == DB || currentEntry->dataType == ASCIZ)
    {
        currentVariable = &((char *)currentEntry->data)[currentIndex];
        fprintf(fileDescriptor,"%02X ", (*(char *)currentVariable) & 0xFF);
    }
    else if(currentEntry->dataType == DH)
    {
        currentVariable = &((short *)currentEntry->data)[currentIndex];
        exportDHByte(currentVariable, printedVariableBytes, fileDescriptor);
    }
    else if(currentEntry->dataType == DW)
    {
        currentVariable = &((int *)currentEntry->data)[currentIndex];
        exportDWByte(currentVariable, printedVariableBytes, fileDescriptor);
    }
}


void exportDataVariable(dataTable currentEntry,long *currentAddress,int currentIndex,FILE *fileDescriptor, long *totalPrintedBytes)
{
    /* The number of bytes printed from the current variable */
    int printedVariableBytes = 0;

    while(printedVariableBytes < currentEntry->variableSize)
    {
        exportDataByteByType(currentEntry, printedVariableBytes, currentIndex, fileDescriptor);
        printedVariableBytes++;
        (*totalPrintedBytes)++;
        /* Starts a new line every time 4 bytes are printed */
        if((*totalPrintedBytes) % 4 == 0)
        {
            /* Increases the new line address by 4 - As the number of bytes printed in the previous line */
            (*currentAddress) += 4;
            /* Every new line, first, prints the new line address */
            fprintf(fileDescriptor,"\n%04lu ", *currentAddress);
        }
    }
}


void exportDataEntry(dataTable currentEntry, long *currentAddress, long *totalPrintedBytes, FILE *fileDescriptor)
{
    int currentIndex;

    for(currentIndex = 0; currentIndex < currentEntry->numOfVariables; currentIndex++)
        exportDataVariable(currentEntry, currentAddress, currentIndex, fileDescriptor, totalPrintedBytes);
}


void exportDataImage(dataTable dataImage, FILE *fileDescriptor, long startingAddress)
{
    dataTable currentEntry;
    long totalPrintedBytes = 0;

    /* First prints the starting address of the data image */
    fprintf(fileDescriptor,"%04lu ", startingAddress);

    for(currentEntry = dataImage; currentEntry != NULL; currentEntry = currentEntry->next)
        exportDataEntry(currentEntry, &startingAddress, &totalPrintedBytes, fileDescriptor);
}


void exportCodeImage(codeTable codeImage, FILE *fd)
{
    /* w - binary representation of the current data from the code image */
    unsigned int *w, b;
    codeTable codeEntry;

    /* hexadecimal representation for byte of 1's in binary */
    b = 0xFF;

    for(codeEntry = codeImage; codeEntry != NULL; codeEntry = codeEntry->next)
    {
        /* In each type of instruction, a different bit field is filled */
        if(codeEntry->type == R)
            w = (unsigned int *)&codeEntry->data->typeR;
        else if(codeEntry->type == I)
            w = (unsigned int *)&codeEntry->data->typeI;
        else if(codeEntry->type == J)
            w = (unsigned int *)&codeEntry->data->typeJ;

        /* Print Format: 'address' '0-7 bits' '15-8 bits' '23-16 bits' '31-24 bits' */
        fprintf(fd,"%04d %02X %02X %02X %02X\n",codeEntry->address, (*w) & b, (*w)>>8 & b, (*w)>>16 & b, (*w)>>24 & b);
    }
}


void createObjectFile(char *fileName, codeTable codeImage, dataTable dataImage, long ICF, long DCF)
{
    FILE *fileDescriptor;
    char *objectFileName;

    objectFileName = getFileFullName(fileName, ".ob");
    /* If filed creation succeeded */
    if(openFile(&fileDescriptor, objectFileName, "w") == VALID)
    {
        /*
          The first line indicates the sizes of the instruction and data images.
          The data image, starting from address 100, is sized by subtracting 100 from its final address.
        */
        fprintf(fileDescriptor, "%ld %ld\n", ICF - 100, DCF);
        exportCodeImage(codeImage, fileDescriptor);
        exportDataImage(dataImage, fileDescriptor, ICF);

        fclose(fileDescriptor);
    }
    free(objectFileName);
}


void exportToExtAndEntFiles(attributesTable attributesTab, FILE *externFileDescriptor, FILE  *entryFileDescriptor)
{
    attributesTable currentEntry;

    /* Exports the current entry to the required file according to the type of the entry - external/entry */
    for(currentEntry = attributesTab; currentEntry != NULL; currentEntry = currentEntry->next)
    {
        if(currentEntry->type == external)
            fprintf(externFileDescriptor, "%s %04ld\n", currentEntry->name, currentEntry->address);
        else if(currentEntry->type == entry)
            fprintf(entryFileDescriptor, "%s %04ld\n", currentEntry->name, currentEntry->address);
    }
}


void createExtAndEntFiles(char *fileName, attributesTable attributesTab)
{
    FILE *externFileDescriptor, *entryFileDescriptor;
    char *externFileName, *entryFileName;
    state externOpen, entryOpen;

    /* Obtains the names of the files to be created and creates them */
    externFileName = getFileFullName(fileName, ".ext");
    entryFileName = getFileFullName(fileName, ".ent");
    externOpen = openFile(&externFileDescriptor, externFileName, "w");
    entryOpen = openFile(&entryFileDescriptor, entryFileName, "w");

    if(externOpen == VALID && entryOpen == VALID) /* If succeeded to create both files */
    {
        exportToExtAndEntFiles(attributesTab, externFileDescriptor, entryFileDescriptor);
        fclose(externFileDescriptor);
        fclose(entryFileDescriptor);
    }
    free(externFileName);
    free(entryFileName);
}


void createOutputFiles(char *fileName, codeTable codeImage, dataTable dataImage, attributesTable attributesTab,
                       long ICF, long DCF)
{
    /* If there is data to insert into 'ext'/'ent' files */
    if(attributesTab != NULL)
        createExtAndEntFiles(fileName, attributesTab);

    /* If there is data to insert into 'ob' file */
    if(dataImage != NULL || codeImage != NULL)
        createObjectFile(fileName, codeImage, dataImage, ICF, DCF);

    /* Frees the data that was used to create the output files */
    freeTables(codeImage, dataImage, attributesTab);
}