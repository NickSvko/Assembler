
#ifndef UNTITLED_GENERAL_H
#define UNTITLED_GENERAL_H

#include "structs.h"
#include "globals.h"


/*
   This  file includes functions used for memory management, error handling,
   and character manipulation within the context of an assembler program.
*/


/*
    Function: callocWithCheck
    Description: Allocates memory of the specified size.
    Parameters:
        - size: The size of memory to be allocated.
    Returns:
        - A pointer to the allocated memory.
    Notes:
        - Exits the program if memory allocation fails.
*/
void *callocWithCheck(unsigned long size);


/*
    Function: addError
    Description: Allocates memory for the provided error message, and returns a pointer to it.
    Parameters:
       - error: The error message string.
    Returns:
        - A pointer to the allocated memory containing the error message.
*/
char *addError(char *error);


/*
    Function: printLineError
    Description: Prints the first error encountered in the current line.
    Parameters:
        - line: Pointer to the structure representing the current line.
        Returns: void
    Notes:
        - The memory allocated for the error is released after printing the error message.
*/
void printLineError(newLine *line);


/*
    Function: currentState
    Description: Returns the state of the current line.
    Parameters:
        - line: Pointer to the structure representing the current line.
    Returns:
       - state: VALID or INVALID.
*/
state currentState(newLine *line);


/*
    Function: isWhiteSpace
    Description: Checks if the given character is a space, tab, or line break.
    Parameters:
        - thisChar: The character to be checked.
   Returns:
       - bool value: TRUE or FALSE
*/
bool isWhiteSpace(char thisChar);


/*
    Function: skipSpaces
    Description: Skips spaces and tabs in the input line content until reaching a non-space/tab character.
    Parameters:
         - lineContent: The content of the line to be processed.
         - lineIndex: Pointer to the index in the line content.
    Notes:
        - Modifies the line index accordingly.
*/
void skipSpaces(const char *lineContent, int *lineIndex);

#endif