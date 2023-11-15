
#include <string.h>
#include <stdlib.h>
#include "../include/stringProcessing.h"
#include "../include/general.h"


void setSymbolTableEntryValues(long address, imageType type, symbolTable newEntry, char *labelName) {
    /* Assigns the label name and address */
    newEntry->name = labelName;
    newEntry->value = address;
    newEntry->next = NULL;

    /* Identifies the type of symbol table entry */
    if (type == entry) {
        /* Marks symbol table entry as 'entry' */
        newEntry->isEntry = TRUE;
    } else if (type == external) {
        /* Marks symbol table entry as 'external' */
        newEntry->isExternal = TRUE;
        newEntry->type = external;
    } else {
        /* Assigns other types */
        newEntry->type = type;
    }
}


void addToSymbolTable(symbolTable *table, char *symbol, long address, imageType type)
{
    symbolTable newEntry, tempEntry;
    char *labelName;

    /* Allocates memory and initializes the symbol */
    newEntry = (symbolTable) callocWithCheck(sizeof(symbolTableEntry));
    labelName = (char *) callocWithCheck(strlen(symbol) + 1);
    strcpy(labelName, symbol);

    /* Set values for the new symbol table entry */
    setSymbolTableEntryValues(address, type, newEntry, labelName);

    /* If the table is empty, the new entry becomes the first */
    if((*table) == NULL)
        (*table) = newEntry;
    /* Otherwise, append the new entry at the end of the list */
    else
    {
        /* Traverse the list to find the last node */
        for(tempEntry = (*table); tempEntry -> next != NULL; tempEntry = tempEntry -> next);
        tempEntry -> next = newEntry;
    }
}


void setDataEntryValues(directiveType type, int numOfVars, long DC, void* dataArr, int sizeofVar, dataTable newEntry)
{
    newEntry -> dataType = type;
	newEntry -> variableSize = sizeofVar;
    newEntry -> numOfVariables = numOfVars;
    newEntry -> data = dataArr;
    newEntry -> address = DC;
    newEntry -> dataSize = sizeofVar * numOfVars;
    newEntry -> next = NULL;
}


int getSizeOfDataVariable(directiveType type)
{
    int sizeofVariable;
    /* '.db' / '.asciz' receives 1 byte variables */
    if(type == DB || type == ASCIZ)
        sizeofVariable = sizeof(char);
    /* '.dh' receives 2 bytes variables */
    if(type == DH)
        sizeofVariable = sizeof(short);
    /* '.dw' receives 4 bytes variables */
    if(type == DW)
        sizeofVariable = sizeof(int);

    return sizeofVariable;
}


void addToDataImage(directiveType type, int numOfVariables, long* DC, void* dataArray, dataTable* table)
{
    int sizeofVariable;
    dataTable newEntry, tempEntry;

    sizeofVariable = getSizeOfDataVariable(type);
    newEntry = callocWithCheck(sizeof(dataImageEntry));

    setDataEntryValues(type, numOfVariables, *DC, dataArray, sizeofVariable, newEntry);
    /* Increases the data counter (DC) according to the size of the obtained data */
    (*DC) += newEntry->dataSize;

    /* If the table is empty */
    if((*table) == NULL)
        (*table) = newEntry;
    /* Defines the new entry to be the last in the list */
    else
    {
        for(tempEntry = (*table); tempEntry -> next != NULL; tempEntry = tempEntry -> next);
        tempEntry -> next = newEntry;
    }
}


void setJBitField(const char *content, int *index, instructionWord *instructionToken, codeTable newEntry)
{
    newEntry -> data -> typeJ.opcode = (*instructionToken).opcode;

    /* If it's 'jump' instruction with register operand */
    if((*instructionToken).opcode == 30 && isRegister(content, (*index)))
    {
        newEntry -> data -> typeJ.reg = 1;
        newEntry -> data -> typeJ.address = getRegister(content, index);
    }
    else
        newEntry -> data -> typeJ.reg = 0;
}


void setIBitField(const char *content, int *index, instructionWord *instructionToken, codeTable newEntry)
{
    newEntry -> data -> typeI.opcode = (*instructionToken).opcode;
    newEntry -> data -> typeI.rs = getRegister(content, index);

    /* Checks if it's a conditional branching instruction */
    if(!((*instructionToken).opcode >= 15 && (*instructionToken).opcode <= 18))
	{
        /* skips to the start of the next operand */
		(*index)++;
		newEntry -> data -> typeI.immed = get2BytesInt(content, index);
	}
    newEntry -> data -> typeI.rt = getRegister(content, index);
}


void setRBitField(const char *content, int *index, instructionWord *instructionToken, codeTable newEntry)
{
    newEntry -> data -> typeR.unused = 0;
    newEntry -> data -> typeR.opcode = (*instructionToken).opcode;
    newEntry -> data -> typeR.funct = (*instructionToken).funct;
    newEntry -> data -> typeR.rs = getRegister(content, index);

    /* If it's a copy instruction, rt field doesn't receive a variable */
    if((*instructionToken).opcode == 1)
        newEntry -> data -> typeR.rt = 0;
    else
        newEntry -> data -> typeR.rt = getRegister(content, index);

    newEntry -> data -> typeR.rd = getRegister(content, index);
}


void setCodeEntryBitfield(const char *content, int index, instructionWord instructionToken, codeTable newEntry)
{
    /* Allocation new memory for the binary representation(using bit field) of the new code image entry */
    newEntry->data = callocWithCheck(sizeof(typeRInstruction));

    if(instructionToken.type == R)
        setRBitField(content, &index, &instructionToken, newEntry);

    else if(instructionToken.type == I)
        setIBitField(content, &index, &instructionToken, newEntry);

    else if(instructionToken.type == J)
        setJBitField(content, &index, &instructionToken, newEntry);
}


void setCodeEntryValues(const char* content, int index, instructionWord instructionToken, long IC, codeTable newEntry,
		long lineNumber)
{
    setCodeEntryBitfield(content, index, instructionToken, newEntry);
	newEntry ->lineNumber = lineNumber;
    newEntry -> address = (int)IC;
    newEntry -> type = instructionToken.type;
    newEntry -> next = NULL;
}


void addToCodeImage(const char* content, int index, instructionWord instructionToken, codeTable* table, long* IC,
		long lineNumber)
{
    codeTable newEntry, tempEntry;

    newEntry = callocWithCheck(sizeof(codeImageEntry));
	setCodeEntryValues(content, index, instructionToken, *IC, newEntry, lineNumber);
    (*IC) += 4;

    /* If the table is empty */
    if((*table) == NULL)
        (*table) = newEntry;
    else
    {
        /* Defines the new entry to be the last in the list */
        for(tempEntry = (*table); tempEntry -> next != NULL; tempEntry = tempEntry -> next);
        tempEntry -> next = newEntry;
    }
}


void setAttributeEntryValues(imageType type, long address, attributesTable newEntry, char *currentName)
{
    newEntry -> name = currentName;
    newEntry -> type = type;
    newEntry -> address = address;
    newEntry -> next = NULL;
}


void addToAttributesTable(char *name, imageType type, long address, attributesTable *table)
{
    attributesTable newEntry, tempEntry;
    char *currentName;

    newEntry = (attributesTable) callocWithCheck(sizeof(attributesTableEntry));
    currentName = (char *) callocWithCheck(strlen(name) + 1);
    strcpy(currentName, name);
    setAttributeEntryValues(type, address, newEntry, currentName);

    if((*table) == NULL)  /* If the table is empty */
        (*table) = newEntry;
    else
    {
        /* Defines the new node to be the last */
        for(tempEntry = (*table); tempEntry -> next != NULL; tempEntry = tempEntry -> next);
        tempEntry -> next = newEntry;
    }
}


void addEntrySymbolsToTable(attributesTable *attributesHead, symbolTable symbolHead)
{
    symbolTable symbolEntry = symbolHead;

    for(; symbolEntry != NULL; symbolEntry = symbolEntry->next)
    {
        if(symbolEntry->isEntry)
            addToAttributesTable(symbolEntry->name, entry, symbolEntry->value, attributesHead);
    }
}


void freeSymbolTable(symbolTable head)
{
    symbolTable temp;
    /* Deallocates the memory associated with each node in the linked list. */
    while(head != NULL)
    {
        /* Pointer to the next node */
        temp = head->next;
        /* remove the current node */
        free(head->name);
        free(head);
        /* Move to the next node */
        head = temp;
    }
}


void freeTables(codeTable codeHead, dataTable dataHead, attributesTable attributesHead)
{
    codeTable codeImageTemp;
    dataTable dataImageTemp;
    attributesTable attributesTemp;

    /* Free code image */
    while(codeHead != NULL)
    {
        codeImageTemp = codeHead->next;
        free(codeHead->data);
        free(codeHead);
        codeHead = codeImageTemp;
    }
    /* Free data image */
    while(dataHead != NULL)
    {
        dataImageTemp = dataHead->next;
        free(dataHead);
        dataHead = dataImageTemp;
    }
    /* Free attributes table */
    while(attributesHead != NULL)
    {
        attributesTemp = attributesHead->next;
        free(attributesHead->name);
        free(attributesHead);
        attributesHead = attributesTemp;
    }
}