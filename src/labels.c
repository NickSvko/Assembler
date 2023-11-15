
#include <string.h>
#include <ctype.h>
#include "../include/stringProcessing.h"
#include "../include/instructions.h"
#include "../include/directives.h"
#include "../include/general.h"
#include "../include/lineHandling.h"


void checkAttributeValidity(newLine *line, imageType type, symbolTable table)
{
    if(table->type == code || table->type == data)
    {
        /* Add 'entry' attribute to defined label */
        if(type == entry)
            table->isEntry = TRUE;
        /* An attempt to define label more the once */
        else
            line->error = addError("Label is already defined");
    }
    /* An attempt to set an external label as a non-external */
    else if(table->isExternal && type != external)
        line->error = addError("Label is already defined as external");
}


bool labelIsDefined(char *label, newLine *line, symbolTable head, imageType type)
{
    bool isDefined = FALSE;
    symbolTable currentSymbol;

    for (currentSymbol = head; currentSymbol != NULL ; currentSymbol = currentSymbol->next)
    {
        if(strcmp(currentSymbol->name, label) == 0)
        {
            isDefined = TRUE;
            checkAttributeValidity(line, type, currentSymbol);
			break;
        }
    }
    /* An attempt of adding 'entry' attribute to a label that doesn't exist */
    if(type == entry && !isDefined)
        line->error = addError("No label was found to be defined as entry");

    return isDefined;
}


bool labelNameIsReservedWord(char *label)
{
    /* The total numbers of the reserved directives and instructions */
    int i, j, numberOfInstructions, numberOfDirectives;
    directiveWord *directive;
    instructionWord *instruction;
    bool isReserved = FALSE;

    directive = getReservedDirectives(&numberOfDirectives);
    instruction = getReservedInstructions(&numberOfInstructions);

    /* Looking for a match between the label name and one of the instructions name */
    for(i = 0; i < numberOfInstructions && isReserved == FALSE; i++)
    {
        if(strcmp(label, instruction[i].name) == 0)
            isReserved = TRUE;
    }
    /* Looking for a match between the label name and one of the directives name */
    for(j = 0; j < numberOfDirectives && isReserved == FALSE; j++)
    {
        if(strcmp(label, directive[j].name) == 0)
            isReserved = TRUE;
    }
    return isReserved;
}


state labelIsValid(newLine *line, char *label)
{
    /*
       Checks if the label name is syntactically correct:
         - Does not exceed the maximum length of a label name
         - The first character is a letter,
         - Contain only letters and numbers.
     */
	if(strlen(label) > maxLabelLength)
		line->error = addError("The label name is longer than 31 characters");
	else if(isalpha(label[0]) == 0 || !isAlphanumeric(label))
		line->error = addError("Invalid label, Syntactic error");

	/* Checks if the label name is a reserved instruction or directive word */
	else if(labelNameIsReservedWord(label))
		line-> error = addError("Invalid label, the label name is a reserved instruction/directive word");

    return currentState(line);
}


bool symbolIsLabelDefinition(const char *lineContent, char *symbol, int *contentIndex)
{
    bool isLabel = FALSE;
    int i = (*contentIndex);
    int symIndex = 0;

    /* Scans the word until encounter end of label definition or end of line */
    for(; lineContent[i] != ':' && lineContent[i] != '\n'; i++, symIndex++)
        symbol[symIndex] = lineContent[i];

    /* If it's a label definition, update line content index to the end of the definition and saves symbol as string */
    if(lineContent[i] == ':')
    {
        /* End of string */
        symbol[symIndex] = '\0';
        /* Promotes the index to be after the symbol definition */
        (*contentIndex) = i + 1;
        isLabel = TRUE;
    }
    return isLabel;
}


void getLabelName(const char *content, int *index, char *label)
{
    int i = 0;
    skipSpaces(content, index);

    /* Scans the label */
    while(!isWhiteSpace(content[*index]) && content[*index] != ',')
        label[i++] = content[(*index)++];

    label[i] = '\0'; /* End of string */
}


void skipLabelDefinition(const char *content, int *index)
{
    int i;
    /* Advance until encounter the end of a label definition or the end of the current word */
    for(i = 0; content[i] != ':' && !isWhiteSpace(content[i]); i++);
    /* If encounter a label setting, skip the label */
    if(content[i] == ':')
    {
        i++;
        (*index) = i;
    }
}


void extractLabelFromLine(char *symbol, const char *content, int index)
{
    /* Advance until encounter the beginning of a label */
    for(; !isalpha(content[index]); index++);

    getLabelName(content, &index, symbol);
}


state getLabelFromTable(newLine *line, char *symbol, symbolTable *label, symbolTable table)
{
    symbolTable currentEntry;

    for(currentEntry = table; currentEntry != NULL; currentEntry = currentEntry->next)
    {
        if(strcmp(symbol, currentEntry->name) == 0)
        {
            (*label) = currentEntry;
            return VALID;
        }
    }
    /* If failed to find the required label */
    line->error = addError("The required label wasn't found");
    return INVALID;
}


void checkForLabelSetting(newLine *line, char *symbol, int *contentIndex, bool *labelSetting)
{
    /* If the first word in line is a valid label definition turns on 'labelSetting' flag. */
    if(symbolIsLabelDefinition(line->content, symbol, contentIndex) && labelIsValid(line, symbol))
        *labelSetting = TRUE;

    /* If the current line is empty after label definition */
    if(*labelSetting == TRUE && emptyLine(line->content, *contentIndex))
        line->error = addError("Missing instruction/directive after label definition");
}


void defineLabelAsEntry(newLine *line, symbolTable symbolTab, int *index, char *label)
{
    getLabelName(line->content, index, label);
    labelIsDefined(label, line, symbolTab, entry);
}


