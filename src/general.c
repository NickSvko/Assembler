
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/structs.h"


void *callocWithCheck(unsigned long size)
{
    void *temp = calloc(1, size);

    if (temp == NULL)
    {
        fprintf(stderr,"Error: Memory allocation failed.");
        exit(1);
    }
    return temp;
}


char *addError(char *error)
{
    char *currentError;
    currentError = callocWithCheck(strlen(error) + 1);
    strcpy(currentError, error);
    return currentError;
}


void printLineError(newLine *line)
{
    fprintf(stderr,"Error! file '%s' line %ld: %s.\n", line-> sourceFileName, line-> number, line-> error);
    /* After printing an error,releases the memory allocated for the error */
    free(line->error);
}


state currentState(newLine *line)
{
    if(line->error)
        return INVALID;
    return VALID;
}


bool isWhiteSpace(char thisChar)
{
    if(thisChar == ' ' || thisChar == '\t' || thisChar == '\n')
        return TRUE;
    return FALSE;
}


void skipSpaces(const char *lineContent,int *lineIndex)
{
    while(lineContent[*lineIndex] == ' ' || lineContent[*lineIndex] == '\t')
        (*lineIndex)++;
}


