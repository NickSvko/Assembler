
#ifndef UNTITLED_TABLES_H
#define UNTITLED_TABLES_H

#include "structs.h"

/*
  This file includes the functions used in assembling and managing data structures relevant to the assembler.
  It contains functions primarily responsible for handling symbol table, code table, data table and attributes table.
 */

/****************  Symbol Table Functions ****************/

/*
   Function: setSymbolTableEntryValues
   Description: Sets values for a symbol table entry (which represented by linked-list).
   Parameters:
     - address: The address associated with the symbol.
     - type: The image type of the symbol (code, data, entry, external).
     - newEntry: Pointer to the symbol table entry to be initialized.
     - labelName: The name of the label.
   Returns:
     - void
   Notes:
   - Initializes the symbol table entry pointed by 'newEntry' with the provided address and label name.
   - Identifies the type of the symbol table entry based on the 'type' parameter:
       - If 'type' is 'entry', marks the entry as an 'entry' type.
       - If 'type' is 'external', marks the entry as an 'external' type.
       - Otherwise, assigns the 'type' to the symbol table entry.
*/
void setSymbolTableEntryValues(long address, imageType type, symbolTable newEntry, char *labelName);


/*
   Function: addToSymbolTable
   Description: Adds an entry to the symbol table (which represented by linked-list).
   Parameters:
     - table: Pointer to the symbol table.
     - symbol: The symbol to add.
     - address: The address associated with the symbol.
     - type: The image type of the symbol (code, data, entry, external).
   Returns:
     - void
   Notes:
   - Allocates memory for a new symbol table entry and initializes its values using 'setSymbolTableEntryValues' function.
   - If the table is empty, the new entry becomes the first element in the list.
   - If the table is not empty, traverses the existing list to find the last node and appends the new entry at the end.

*/
void addToSymbolTable(symbolTable *table, char *symbol, long address, imageType type);


/*
   Function: freeSymbolTable
   Description: Releases the memory allocated for the symbol table.
   Parameters:
     - head: head Pointer to the head of the symbol table.
   Returns:
     - void
   Notes:
     - Iterates through all elements of the symbol table (which are represented as nodes in a linked list),
       and deallocates the memory associated with each of them.
 */
void freeSymbolTable(symbolTable head);



/**************** Data Table Functions ****************/

/*
   Function: setDataEntryValues
   Description: Sets the values of a new data image entry.
   Parameters:
     - type: The directive type for the data image entry (DH, DW, DB, ASCIZ, ENTRY, EXTERN, NONE).
     - numOfVars: The number of variables associated with this data entry.
     - DC: The address where the data is to be stored.
     - dataArr: Pointer to the data array to be stored.
     - sizeofVar: The size of each variable in bytes.
     - newEntry: Pointer to the data image entry to be initialized.
   Returns:
     - void
   Notes:
     - Assigns the directive type, variable size, number of variables, address, and data array
       to the respective fields in the data entry structure.
     - Calculates and assigns the total size of the data entry based on the size of a single variable
       and the number of variables.
     - Initializes the 'next' pointer of the data entry as NULL.
*/
void setDataEntryValues(directiveType type, int numOfVars, long DC, void* dataArr, int sizeofVar, dataTable newEntry);


/*
   Function: getSizeOfDataVariable
   Description: Determines the size of a single data variable based on the directive type.
   Parameters:
     - type: The directive type that determines the size of the variable.
   Returns:
     - The size of a single variable based on the directive type (1, 2, or 4 bytes).
   Notes:
     - Checks the directive type and assigns the size accordingly: '.db' or '.asciz' receives 1 byte variables,
       '.dh' receives 2 bytes variables, and '.dw' receives 4 bytes variables.
     - Returns the size of the variable based on the directive type provided.
*/
int getSizeOfDataVariable(directiveType type);


/*
   Function: addToDataImage
   Description: Adds a new entry to the data image linked list.
   Parameters:
     - type: The directive type for the data image entry ('.db', '.dh', '.dw', '.asciz').
     - numOfVariables: The number of variables to add.
     - DC: Pointer to the Data Counter used for addressing.
     - dataArray: Pointer to the array of data variables to be added.
     - table: Pointer to the data table to which the entry will be added.
   Returns:
     - void
   Notes:
     - The function calculates the size of each variable based on the directive type.
     - It allocates memory for the new data entry and sets its values using setDataEntryValues().
     - Increases the Data Counter (DC) according to the size of the added data.
*/
void addToDataImage(directiveType type, int numOfVariables, long* DC, void* dataArray, dataTable* table);



/**************** Code Table Functions ****************/

/*
   Function: setCodeEntryValues
   Description: Initializes a new code image entry with values from the instruction and content.
   Parameters:
     - content: Instruction line content.
     - index: Index of the content being processed.
     - instructionToken: Structure containing instruction details.
     - IC: Current instruction counter value.
     - newEntry: Pointer to codeTable entry to be initialized.
     - lineNumber: Line number associated with the instruction.
   Returns:
     - void
   Notes:
     - Initializes the code entry's binary representation.
     - Sets the line number, address, instruction type, and initializes next to NULL.
*/
void setCodeEntryValues(const char* content, int index, instructionWord instructionToken, long IC, codeTable newEntry, long lineNumber);


/*
   Function: addToCodeImage
   Description: Adds a new code image entry to the linked list.
   Parameters:
     - content: Instruction line content.
     - index: Index of the content being processed.
     - instructionToken: Structure containing instruction details.
     - table: Pointer to the head of the code image linked list.
     - IC: Pointer to the current instruction counter value.
     - lineNumber: Line number associated with the instruction.
   Returns:
     - void
   Notes:
     - Increments the instruction counter by 4 bytes.
     - Appends the new entry to the end of the code image linked list.
*/
void addToCodeImage(const char* content, int index, instructionWord instructionToken, codeTable* table, long* IC, long lineNumber);


/*
   Function: setCodeEntryBitfield
   Description: Sets the binary representation of an instruction line based on its type.
   Parameters:
     - content: Instruction line content.
     - index: Index of the content being processed.
     - instructionToken: Structure containing instruction details.
     - newEntry: Pointer to codeTable entry for the binary representation.
   Returns:
     - void
   Notes:
     - Allocates memory for the binary representation of the code image entry.
     - Calls specific functions to set the bitfield based on the instruction type (R, I, or J).
*/
void setCodeEntryBitfield(const char *content, int index, instructionWord instructionToken, codeTable newEntry);


/*
   Function: setRBitField
   Description: Sets the binary representation of a R-type instruction line.
   Parameters:
     - content: Instruction line content.
     - index: Pointer to the content index being processed.
     - instructionToken: Pointer to instructionWord containing instruction details.
     - newEntry: Pointer to codeTable entry for the binary representation.
   Returns:
     - void
   Notes:
     - Assigns opcode, funct, rs, rt, and rd fields in newEntry from content and instructionToken.
     - Adjusts rt field if it's a copy instruction.
*/
void setRBitField(const char *content, int *index, instructionWord *instructionToken, codeTable newEntry);


/*
   Function: setIBitField
   Description: Sets the binary representation of an 'I' type instruction line.
   Parameters:
     - content: The content of the instruction line.
     - index: Pointer to the index of the content being processed.
     - instructionToken: Pointer to the instructionWord structure containing the instruction details.
     - newEntry: Pointer to the codeTable entry to store the generated binary representation.
   Returns:
     - void
   Notes:
     - Assigns opcode from instructionToken to newEntry.
     - Retrieves and assigns rs from content using the index.
     - For conditional branches, skips to the next operand and assigns immed.
     - Retrieves and assigns rt from content using the index.
*/
void setIBitField(const char *content, int *index, instructionWord *instructionToken, codeTable newEntry);


/*
   Function: setJBitField
   Description: Sets the binary representation of a J-type instruction line.
   Parameters:
     - content: The content of the instruction line.
     - index: Pointer to the index of the content being processed.
     - instructionToken: Pointer to the instructionWord structure containing the instruction details.
     - newEntry: Pointer to the codeTable entry to store the generated binary representation.
   Returns:
     - void
   Notes:
     - Assigns opcode from instructionToken to newEntry's opcode field.
     - If it's a 'jump' instruction with a register operand, sets the reg field and retrieves the register's address.
*/
void setJBitField(const char *content, int *index, instructionWord *instructionToken, codeTable newEntry);



/******************** Attributes Table Functions ********************/

/*
   Function: setAttributeEntryValues
   Description: Sets the values of a new attribute table entry.
   Parameters:
     - type: The image type associated with the attribute entry.
     - address: The address value associated with the attribute entry.
     - newEntry: Pointer to the attribute table entry to be initialized.
     - currentName: The name of the attribute entry.
   Returns:
     - void
   Notes:
     - Assigns the name and address values to the new attribute entry.
     - Sets the image type for the attribute entry.
     - Initializes the next pointer to NULL.
*/
void setAttributeEntryValues(imageType type, long address, attributesTable newEntry, char *currentName);


/*
   Function: addToAttributesTable
   Description: Adds a new entry to the attributes linked list.
   Parameters:
     - name: The name associated with the attribute entry.
     - type: The image type associated with the attribute entry.
     - address: The address value associated with the attribute entry.
     - table: Pointer to the head of the attributes linked list.
   Returns:
     - void
   Notes:
     - Allocates memory for a new attribute table entry and initializes its values.
     - Sets the name, image type, and address for the new attribute entry.
     - Appends the new entry to the end of the attributes linked list.
*/
void addToAttributesTable(char *name, imageType type, long address, attributesTable *table);


/*
   Function: addEntrySymbolsToTable
   Description: Adds all the labels marked as 'entry' from the symbol table to the attributes table.
   Parameters:
     - attributesHead: Pointer to the head of the attributes linked list.
     - symbolHead: Pointer to the head of the symbol table linked list.
   Returns:
     - void
   Notes:
     - Iterates through the symbol table entries to identify those marked as 'entry'.
     - For each 'entry' symbol found, adds an attribute entry to the attributes table.
*/
void addEntrySymbolsToTable(attributesTable *attributesHead, symbolTable symbolHead);


/*
   Function: freeTables
   Description: Releases memory allocated for data image, code image, and attributes table linked lists.
   Parameters:
     - codeHead: Pointer to the head of the code image linked list.
     - dataHead: Pointer to the head of the data image linked list.
     - attributesHead: Pointer to the head of the attributes table linked list.
   Returns:
     - void
   Notes:
     - Iterates through each linked list and frees memory associated with each node.
     - Releases memory for the code image, data image, and attributes table linked lists.
*/
void freeTables(codeTable codeHead, dataTable dataHead, attributesTable attributesHead);


#endif