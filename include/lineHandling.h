#ifndef UNTITLED_LINEHANDLING_H
#define UNTITLED_LINEHANDLING_H

#include <stdio.h>
#include "structs.h"


/*
   This file provides functions for handling and analyzing lines from an input file,
   including skipping to the next line, checking line length validity,
   identifying empty or comment lines and printing line error.
*/


/*
    Function: skipToTheNextLine
    Description: Skips to the beginning of the next line in the input file.
    Parameters:
        - fileDescriptor: Pointer to the file being read.
    Returns:
        - void
    Notes:
        - This function reads characters from the file descriptor until the next line or the end of the file.
*/
void skipToTheNextLine(FILE *fileDescriptor);


/*
    Function: lineLength
    Description: Checks if the line length is valid and updates the line state accordingly.
    Parameters:
        - inputLine: Pointer to the content of the line being processed.
        - line: Pointer to the structure representing the current line.
        - fd: Pointer to the file being read.
    Returns:
        - The state of the line. state variable: valid/invalid.
    Notes:
        - This function checks if the length of the line exceeds the maximum allowed length.
        - If the line length is invalid, it adds an error to the line structure and skips to the next line.
*/
state lineLength(const char *inputLine, newLine *line, FILE *fd);


/*
    Function: emptyLine
    Description: Checks if the given line is empty.
    Parameters:
        - lineContent: Pointer to the content of the line being processed.
        - currentIndex: Pointer to the index in the line content.
    Returns:
        - If the given line is empty. bool variable: TRUE/FALSE
*/
bool emptyLine(const char *lineContent, int currentIndex);


/*
    Function: commentLine
    Description: Checks if the given line is a comment line.
    Parameters:
        - lineContent: Pointer to the content of the line being processed.
        - currentIndex: Pointer to the index in the line content.
    Returns:
        - If the given line is a comment line. bool variable: TRUE/FALSE
*/
bool commentLine(const char *lineContent, int *currentIndex);


/*
    Function: printLineError
    Description: Prints an error message associated with the line if there's any error present.
    Parameters:
        - line: Pointer to the structure representing the current line.
    Returns:
        - void
*/
void printLineError(newLine *line);

#endif
