
#ifndef UNTITLED_DIRECTIVES_H
#define UNTITLED_DIRECTIVES_H

#include "structs.h"


/*
  This file serves to facilitate the handling and processing of directives within the input files.
  It includes functions to scan, analyze, and process different types of directives encountered in the code.
*/


/*
    Function: scanDirectiveName
    Description: Scans the current directive word from the input line.
    Parameters:
        - lineContent: The content of the input line.
        - directiveName: The buffer to store the directive name.
        - contentIndex: The index in the content where the directive starts.
    Returns:
        - void
    Notes:
        - Updates the contentIndex to the position after the scanned directive.
*/
void scanDirectiveName(const char *lineContent, char *directiveName, int *contentIndex);


/*
    Function: isDirective
    Description: Checks if the current word from the input line is a directive definition.
    Parameters:
        - lineContent: The content of the input line.
        - directiveToken: Pointer to the directiveWord structure to store the directive details if found.
        - contentIndex: Pointer to the index in the content where the search begins.
    Returns:
        - bool: Indication whether the word represents directive definition or not.
*/
bool isDirective(const char *lineContent, directiveWord *directiveToken, int *contentIndex);


/*
    Function: searchDirective
    Description: Searches for a match between the current word and one of the available directive words.
    Parameters:
        - line: Pointer to the newLine structure containing the line information.
        - directiveToken: Pointer to the directiveWord structure to store the matched directive details.
    Returns:
        - state: The state of the search operation (VALID or INVALID).
*/
state searchDirective(newLine *line, directiveWord  *directiveToken);


/*
    Function: isDataStorageDirective
    Description: Checks whether the given directive type represents a data storage directive.
    Parameters:
        - type: The directiveType to be checked.
    Returns:
        - bool: Indication whether the given directive is a data storage directive or not.
*/
bool isDataStorageDirective(directiveType type);


/*
    Function: enterVariableByType
    Description: Inserts the current variable into the data array based on the directive type.
    Parameters:
        - type: The directiveType specifying the type of directive.
        - dataArray: Pointer to the data array.
        - arrayIndex: Pointer to the index in the array where the variable will be inserted.
        - numString: String containing the variable value to be inserted.
    Returns:
        - void
*/
void enterVariableByType(directiveType type, void * dataArray, int *arrayIndex, char *numString);


/*
    Function: scanDVariableToArray
    Description: Scans a variable from the input line and enters it into the data array (for '.db'/'dh'/'dw' directives).
    Parameters:
        - content: Pointer to the content of the input line.
        - index: Index in the content indicating the position of the variable.
        - type: The directiveType specifying the type of directive.
        - dataArray: Pointer to the data array.
    Returns:
        - void
*/
void scanDVariableToArray(const char *content, int index, directiveType type, void *dataArray);


/*
    Function: scanAscizVariableToArray
    Description: Scans a variable from the input line and enters it into the data array for '.asciz' directive.
    Parameters:
        - content: Pointer to the content of the input line.
        - index: Index in the content indicating the position of the variable.
        - dataArray: Pointer to the data array.
    Returns:
        - void
*/
void scanAscizVariableToArray(const char *content, int index, char *dataArray);


/*
    Function: createDataArray
    Description: Creates an array of data based on the directive type and input variables.
    Parameters:
        - type: The directiveType specifying the type of directive.
        - dataArray: Pointer to the pointer of the data array.
        - numOfVariables: Number of variables to be processed.
        - content: Pointer to the content of the input line.
        - index: Index in the content indicating the position of the variable.
    Returns:
        - void
*/
void createDataArray(directiveType type, void **dataArray, int numOfVariables, const char *content, int index);


/*
    Function: checkAscizCharValidity
    Description: Checks the validity of characters for the '.asciz' directive.
    Parameters:
        - line: Pointer to the newLine structure representing the input line.
        - contentIndex: Index in the content of the input line.
        - numOfVariables: Pointer to the number of variables.
        - inQuotes: Boolean indicating whether the character is within quotes.
    Returns:
        - void
*/
void checkAscizCharValidity(newLine *line, int contentIndex, int *numOfVariables, bool inQuotes);


/*
    Function: checkAscizDirectiveLine
    Description: Checks the validity of an input line representing '.asciz' directive.
    Parameters:
        - line: Pointer to the newLine structure representing the input line.
        - contentIndex: Index in the content of the input line.
        - numOfVariables: Pointer to the number of variables.
    Returns:
        - void
*/
void checkAscizDirectiveLine(newLine *line, int contentIndex, int *numOfVariables);


/*
    Function: checkDTypeDirectiveLine
    Description: Checks the validity of an input line representing '.db'/'dh'/'dw' directives.
    Parameters:
        - line: Pointer to the newLine structure representing the input line.
        - thisDirective: The directiveType specifying the type of directive.
        - contentIndex: Index in the content of the input line.
        - numOfVariables: Pointer to the number of variables.
    Returns:
        - void
*/
void checkDTypeDirectiveLine(newLine *line, directiveType thisDirective, int contentIndex, int *numOfVariables);


/*
    Function: checkDirectiveByType
    Description: Checks an input line for a specific directive type.
    Parameters:
        - line: Pointer to the newLine structure representing the input line.
        - type: The directiveType specifying the type of directive to be checked.
        - index: Index in the content of the input line.
        - numOfVariables: Pointer to the number of variables.
    Returns:
        - void
*/
void checkDirectiveByType(newLine *line, directiveType type, int index, int *numOfVariables);


/*
    Function: dataStorageDirective
    Description: Handles the data storage directive based on its type.
    Parameters:
        - line: Pointer to the newLine structure representing the input line.
        - type: The directiveType specifying the type of directive.
        - index: Index in the content of the input line.
        - numOfVariables: Pointer to the number of variables.
        - dataArray: Pointer to the pointer of the data array.
    Returns:
        - state: State of the data storage directive processing.
    Notes:
        - If the directive is valid, Produces an array using 'createDataArray' function.
        - If the data storage directive invalid, stores proper error in 'line'.
*/
state dataStorageDirective(newLine *line, directiveType type, int index, int *numOfVariables, void **dataArray);


/*
    Function: processDataStorageDirective
    Description: Processes an input line representing data storage directive.
    Parameters:
        - label: Pointer to the label associated with the directive.
        - line: Pointer to the newLine structure representing the input line.
        - type: The directiveType specifying the type of directive.
        - labelSet: Boolean indicating whether a label is set.
        - index: Index in the content of the input line.
        - symTable: Pointer to the symbol table.
        - dataImage: Pointer to the data image table.
        - DC: Pointer to the Data Counter.
    Returns:
        - void
*/
void processDataStorageDirective(char *label, newLine *line, directiveType type, bool labelSet, int index, symbolTable *symTable, dataTable *dataImage, long *DC);


/*
    Function: processDirective
    Description: Processes an input line representing a directive.
    Parameters:
        - directToken: Pointer to the directiveWord structure representing the directive token.
        - labelSet: Boolean indicating whether a label is set.
        - line: Pointer to the newLine structure representing the input line.
        - index: Pointer to the index in the content of the input line.
        - DC: Pointer to the Data Counter.
        - symTab: Pointer to the symbol table.
        - dImage: Pointer to the data image table.
        - label: Pointer to the label associated with the directive.
    Returns:
        - void
    Notes:
        - Does not process '.entry' directive.
*/
void processDirective(directiveWord *directToken, bool labelSet, newLine *line, int *index, long *DC, symbolTable *symTab, dataTable *dImage, char *label);


/*
    Function: getReservedDirectives
    Description: Retrieves the list of reserved directives.
    Parameters:
        - numberOfDirectives: Pointer to an integer to hold the number of directives.
    Returns:
        - *directiveWord: The list of reserved directives.
    Notes:
        - Stores the current number of directives in 'numberOfDirectives' variable.
        - Each directive in the table contains information about its name, opcode, funct and dataType.
*/
directiveWord *getReservedDirectives(int *numberOfDirectives);


#endif