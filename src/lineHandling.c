
#include <string.h>
#include <stdio.h>
#include "../include/general.h"


void skipToTheNextLine(FILE *fileDescriptor)
{
    int temp;
    do
    {
        temp = fgetc(fileDescriptor);
    }
    while(temp != '\n' && temp != EOF);
}


state lineLength(const char *inputLine, newLine *line, FILE *fd)
{
	/* If no '\n' character is found - not the entire line is scanned, meaning the line is longer than the max length */
    if(strchr(inputLine,'\n') == NULL)
	{
		line->error = addError("line is too long");
		skipToTheNextLine(fd);
	}
    return currentState(line);
}


bool emptyLine(const char *lineContent, int currentIndex)
{
    int tempIndex;

    tempIndex = currentIndex;
    skipSpaces(lineContent, &tempIndex);

    if(lineContent[tempIndex] == '\n')
        return TRUE;
    return FALSE;
}


bool commentLine(const char *lineContent, int *currentIndex)
{
    skipSpaces(lineContent, currentIndex);

    if(lineContent[*currentIndex] == ';')
        return TRUE;
    return FALSE;
}


