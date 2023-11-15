#ifndef UNTITLED_LABELS_H
#define UNTITLED_LABELS_H

#include "structs.h"


/*
   This file contains functions related to label processing in the assembler program.
   The functions responsible for handling labels, checking for reserved words,
   and managing attributes associated with labels in the assembly code.
 */


/*
    Function: checkAttributeValidity
    Description: Checks if the current symbol can be added to the current label.
    Parameters:
        - line: Pointer to the structure representing the current line.
        - type: The type of image.
        - table: The symbol table.
    Returns:
        - void
*/
void checkAttributeValidity(newLine *line, imageType type, symbolTable table);


/*
    Function: labelIsDefined
    Description: Checks and returns if the current label is defined in the symbol table.
    Parameters:
        - label: Pointer to the label string.
        - line: Pointer to the structure representing the current line.
        - head: Pointer to the symbol table head.
        - type: The image type.
    Returns:
        - If the given label is defined. bool variable: TRUE / FALSE.
*/
bool labelIsDefined(char *label, newLine *line, symbolTable head, imageType type);


/*
    Function: labelIsValid
    Description: Checks the validation of a label.
    Parameters:
        - line: Pointer to the structure representing the current line.
        - label: Pointer to the label string.
    Returns:
        - The state of the label. state variable: VALID / INVALID.
*/
state labelIsValid(newLine *line, char *label);


/*
    Function: symbolIsLabelDefinition
    Description: Checks if the string that starts in given index of the given represents label definition.
    Parameters:
        - lineContent: Pointer to the content of the line.
        - symbol: Pointer to the symbol string.
        - contentIndex: Pointer to the index of the content.
    Returns:
        - If it's a label definition. bool variable: TRUE / FALSE.
*/
bool symbolIsLabelDefinition(const char *lineContent, char *symbol, int *contentIndex);


/*
    Function: getLabelName
    Description: Scans the next label from the content.
    Parameters:
        - content: Pointer to the content string.
        - index: Pointer to the current index in the content.
        - label: Pointer to store the extracted label string.
    Returns:
        - void
    Notes:
        - Extracts the label from the content at the current index position into 'label'.
*/
void getLabelName(const char *content, int *index, char *label);


/*
    Function: skipLabelDefinition
    Description: Skips the label definition in the content.
    Parameters:
        - content: Pointer to the content string.
        - index: Pointer to the current index in the content.
    Returns:
        - void
    Notes:
        - Skips the label definition (if any) and updates the index accordingly.
*/
void skipLabelDefinition(const char *content, int *index);


/*
    Function: extractLabelFromLine
    Description: Finds and extracts the label from the content.
    Parameters:
        - symbol: Pointer to store the extracted label string.
        - content: Pointer to the content string.
        - index: The current index in the content.
    Returns:
        - void
    Notes:
        - Searches for a label in the content and extracts it to the 'symbol' variable.
*/
void extractLabelFromLine(char *symbol, const char *content, int index);


/*
    Function: getLabelFromTable
    Description: Trying to extract label from the symbol table.
    Parameters:
        - line: Pointer to the structure representing the current line.
        - symbol: Pointer to the string representing label's name.
        - label: Pointer to the storage location of the label, if found.
        - table: The symbol table.
    Returns:
        - If the required label has been found. state variable - VALID / INVALID.
    Notes:
        - Searches for a label in the symbol table by the name stored in 'symbol' and retrieves it into 'label'.
        - If failed to find the required label, adds the proper error to the line object.
*/
state getLabelFromTable(newLine *line, char *symbol, symbolTable *label, symbolTable table);


/*
    Function: checkForLabelSetting
    Description: Checks for valid label setting in the line content.
    Parameters:
        - line: Pointer to the structure representing the current line.
        - symbol: Pointer to the symbol string.
        - contentIndex: Pointer to the current content index.
        - labelSetting: Pointer to a boolean flag indicating label setting.
    Returns:
        - void
    Notes:
        - Verifies if the line contains a valid label definition and sets the labelSetting flag accordingly.
*/
void checkForLabelSetting(newLine *line, char *symbol, int *contentIndex, bool *labelSetting);


/*
    Function: defineLabelAsEntry
    Description: Adds 'entry' attribute to a defined label.
    Parameters:
        - line: Pointer to the structure representing the current line.
        - symbolTab: Symbol table.
        - index: Pointer to the current index.
        - label: Pointer to the label string.
    Returns:
        - void
    Notes:
        - Marks the given label as an 'entry' in the symbol table.
*/
void defineLabelAsEntry(newLine *line, symbolTable symbolTab, int *index, char *label);


/*
    Function: labelNameIsReservedWord
    Description: Checks if the label name is a reserved word representing an instruction or directive.
    Parameters:
        - label: Pointer to the label string.
    Returns:
        - If the label name is a reserved instruction/directive word. bool variable: TRUE / FALSE.
    Notes:
        - The determination is carried out by iterating over all the instruction and directive words,
          and checking whether the given label is equal to one of them.
*/
bool labelNameIsReservedWord(char *label);


#endif
