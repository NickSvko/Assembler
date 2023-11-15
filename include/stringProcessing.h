#ifndef UNTITLED_UTILS_H
#define UNTITLED_UTILS_H

#include "structs.h"


/*
   This  file provides a collection of utility functions used for processing and analyzing strings,
   within the context of assembly language parsing and handling.
*/


/*
    Function: isAlphanumeric
    Description: Checks if the given symbol is alphanumeric.
    Parameters:
        - symbol: A pointer to the symbol to be checked.
    Returns:
        - If the symbol is alphanumeric. bool variable: TRUE / FALSE
*/
bool isAlphanumeric(const char *symbol);


/*
    Function: checkForComma
    Description: Checks the validity of a comma in the current position and returns the syntax's validity.
    Parameters:
        - line: Pointer to the line structure.
        - index: Pointer to the index in the line content.
        - numOfVariables: Number of variables encountered.
    Returns:
        - The syntax validity. state variable: VALID / INVALID.
*/
state checkForComma(newLine *line, int *index, int numOfVariables);


/*
    Function: checkInteger
    Description: Extracts the next integer in the line and validates its range.
    Parameters:
        - line: Pointer to the structure representing the current line.
        - contentIndex: Pointer to the index in the line's content.
        - numOfVariables: Pointer to the count of variables encountered.
        - maxNumLength: Maximum length of the integer to be extracted.
        - maxVal: Maximum allowed value for the integer.
        - minVal: Minimum allowed value for the integer.
    Returns:
        - void
    Notes:
        - This function extracts an integer from the line's content at the specified index.
        - It validates whether the extracted integer falls within the specified range.
        - If the integer is valid, it increments the count of encountered variables.
*/
void checkInteger(newLine* line, int* contentIndex, int* numOfVariables, int maxNumLength, int maxVal, int minVal);


/*
    Function: checkRegister
    Description: Extracts and validates the presence of a register in the line's content.
    Parameters:
        - line: Pointer to the structure representing the current line.
        - contentIndex: Pointer to the index in the line's content.
        - numOfScannedOperands: Pointer to the count of scanned operands.
    Returns:
        - void
    Notes:
        - This function checks for the presence of a register in the line's content at the specified index.
        - It verifies if the register starts with '$' and represents a valid register number (0-31).
        - If a valid register is found, it increments the count of scanned operands.
*/
void checkRegister(newLine *line, int *contentIndex, int *numOfScannedOperands);


/*
    Function: checkOperandsAmount
    Description: Validates the number of operands based on the opcode and the count of scanned operands.
    Parameters:
        - line: Pointer to the structure representing the current line.
        - opcode: The opcode representing the specific instruction type.
        - numOfScannedOperands: The count of operands already scanned.
        - endOfScan: Boolean indicating if it's the end of scanning the line.
    Returns:
        - void
    Notes:
        - This function verifies if the number of scanned operands matches the expected count for a given opcode.
        - It checks the validity of the operand count against opcode specifications.
        - If the count of scanned operands does not match the expected count, it adds an error to the line's error message.
*/
void checkOperandsAmount(newLine* line, unsigned int opcode, int numOfScannedOperands, bool endOfScan);


/*
    Function: getRegister
    Description: Extracts the register number from the content at the specified index.
    Parameters:
        - content: Pointer to the string containing the content to extract the register from.
        - index: Pointer to the current index position within the content.
    Returns:
        - The extracted register number. int variable.
    Notes:
        - This function searches for and extracts the register number from the given content.
        - It navigates the content based on the index, identifying the register number.
        - Returns the extracted register number found in the content at the specified index.
*/
int getRegister(const char *content, int *index);


/*
    Function: get2BytesInt
    Description: Extracts a 2-byte integer from the content at the specified index.
    Parameters:
        - content: Pointer to the string containing the content to extract the integer from.
        - index: Pointer to the current index position within the content.
    Returns:
        - An integer representing the extracted 2-byte integer.
    Notes:
        - This function scans the content at the provided index to extract a 2-byte integer.
        - It navigates the content based on the index and retrieves the integer value.
        - Returns the extracted 2-byte integer found in the content at the specified index.
*/
int get2BytesInt(const char *content, int *index);


/*
    Function: isRegister
    Description: Checks whether the given content from the given position contains a valid register.
    Parameters:
        - content: Pointer to the string containing the content to check.
        - index: Index position within the content to start checking.
    Returns:
        - If a valid register been has found. bool variable: TRUE / FALSE.
    Notes:
        - This function scans the content starting from the provided index, searching for a valid register.
        - It checks for the presence of a valid register pattern ('$' followed by a number) at the given index.
        - Returns TRUE if a valid register pattern is found; otherwise, returns FALSE.
*/
bool isRegister(const char *content, int index);


/*
    Function: getFileFullName
    Description: Concatenates the provided file name and file extension to a new string and returns pointer to it.
    Parameters:
        - fileName: Pointer to the string containing the file name.
        - fileExtension: Pointer to the string containing the file extension.
    Returns:
        - A newly allocated string containing the file name with the given extension. String variable.
    Notes:
        - This function allocates memory to store the resulting full file name.
        - It extracts the given filename without its extension and allocates memory to the return string.
        - Concatenates the given file extension to the extracted filename to create the full file name.
        - Returns the newly created full file name string.
*/
char* getFileFullName(char *fileName, char *fileExtension);


/*
    Function: scanInt
    Description: Scans and extracts an integer from the given content starting from the provided contentIndex.
    Parameters:
        - content: Pointer to the string containing the content to be scanned.
        - contentIndex: Pointer to the index indicating the starting point for scanning.
        - numString: Pointer to a character array to store the scanned integer as a string.
        - maxLength: The maximum length expected for the scanned integer string representation.
    Returns:
        - The extracted value. int variable.
    Notes:
        - This function scans the string from the contentIndex character by character until a space, comma, or end of line is encountered.
        - It stores the scanned integer as a string in the numString array.
        - Returns the extracted integer value from the provided content.
*/
int scanInt(const char *content, int *contentIndex, char *numString, int maxLength);


/*
    Function: intIsValid
    Description: Checks the validity of a given string representation of an integer within specified limits.
    Parameters:
        - numString: Pointer to the character array representing the integer as a string.
        - numValue: The integer value extracted from the numString.
        - maxValue: The maximum allowed value for the integer.
        - minValue: The minimum allowed value for the integer.
    Returns:
        - If the string represents a valid integer within the specified limits. bool variable: TRUE / FALSE.
    Notes:
        - This function verifies if the provided numString contains a valid integer representation.
        - It checks if the extracted integer value (numValue) falls within the specified range (minValue to maxValue).
        - Returns 'true' if the integer representation in numString is valid within the specified limits;
          otherwise, returns 'false'.
*/
bool intIsValid(char* numString, int numValue, int maxValue, int minValue);


#endif
