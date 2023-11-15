
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../include/stringProcessing.h"
#include "../include/directives.h"
#include "../include/general.h"
#include "../include/labels.h"
#include "../include/tables.h"
#include "../include/lineHandling.h"


void scanDirectiveName(const char *lineContent, char *directiveName, int *contentIndex)
{
    int nameIndex = 0;

    /* Scans the directive name */
    while(!isWhiteSpace(lineContent[*contentIndex]))
        directiveName[nameIndex++] = lineContent[(*contentIndex)++];

    directiveName[nameIndex] = '\0'; /* End of string */
}


bool isDirective(const char *lineContent, directiveWord *directiveToken, int *contentIndex)
{
    bool directiveDefinition = FALSE;
    int i;

    skipSpaces(lineContent, contentIndex); /* update contentIndex to the next char that is not ' ' or '\t' */
	i = (*contentIndex);

    /* If the first char of the current word is '.' than it's a directive word declaration. */
    if (lineContent[i] == '.')
    {
        i++; /* Skips the dot to the beginning of the directive name */
        scanDirectiveName(lineContent, directiveToken->name, &i);
        (*contentIndex) = i;
        directiveDefinition = TRUE;
    }
    return directiveDefinition;
}


directiveWord *getReservedDirectives(int *numberOfDirectives)
{
    static directiveWord directive[7] =
            {
                    {"dh", DH},
                    {"dw", DW},
                    {"db", DB},
                    {"asciz", ASCIZ},
                    {"entry", ENTRY},
                    {"extern", EXTERN},
                    {"", NONE }
            };

    *numberOfDirectives = sizeof(directive) / sizeof(directive[0]);
    return directive;
}


state searchDirective(newLine *line, directiveWord  *directiveToken)
{
    /* numberOfDirectives = The total number of the reserved directives */
    int i, numberOfDirectives;
    directiveWord *directive;

    directive = getReservedDirectives(&numberOfDirectives);

    /* Searches for a match between the current word and one of the directives name */
    for(i = 0; i < numberOfDirectives; i++)
    {
        /* if a match is found saves the matched directive dataType and stops */
        if(strcmp(directiveToken->name, directive[i].name) == 0)
        {
            directiveToken->type = directive[i].type;
            break;
        }
    }
    if(i == numberOfDirectives) /* If no match was found */
        line-> error = addError("unrecognized directive word");

    return currentState(line);
}


bool isDataStorageDirective(directiveType type)
{
    if(type == DH || type == DW || type == DB || type == ASCIZ)
        return TRUE;
    return FALSE;
}


void enterVariableByType(directiveType type, void *dataArray, int *arrayIndex, char *numString)
{
    int numValue;
    numValue = atoi(numString);

    if(type == DB)
        ((char *)dataArray)[*arrayIndex] = (char)numValue;  /* '.db' directive contain 1 byte variables */
    else if(type == DH)
        ((short *)dataArray)[*arrayIndex] = (short)numValue;  /* '.dh' directive contain 2 bytes variables */
    else if(type == DW)
        ((int *)dataArray)[*arrayIndex] = numValue;  /* '.dw' directive contain 4 bytes variables */

	(*arrayIndex)++;
}


void scanDVariableToArray(const char *content, int index, directiveType type, void *dataArray)
{
    int i = 0, arrayIndex = 0;
    /* Max length of any valid number from input */
    char numString[max4BytesIntLength];

    /* Scans all the numbers to the end of the line  */
	while(content[index] != '\n')
	{
		/* Advances until encounter a number or the end of a line */
		for(;content[index] == ',' || content[index] == ' ' || content[index] == '\t'; index++);

		/* Scans the number encountered */
		while(!isWhiteSpace(content[index]) && content[index] != ',')
			numString[i++] = (char)(content[index++] + '\0');

        /* End of string */
		numString[i] = '\0';

		/* If a number has been found */
		if(i != 0)
			enterVariableByType(type, dataArray, &arrayIndex, numString);
		i = 0;
	}
}


void scanAscizVariableToArray(const char *content, int index, char *dataArray)
{
    int arrayIndex = 0;

    /* Progress until encounter the beginning of the string */
    for(; content[index] != '"';index++);
    index++;
    /* Progress until encounter the end of the string */
    while(content[index] != '"')
        dataArray[arrayIndex++] = content[index++];

    /* End of string */
    dataArray[arrayIndex] = '\0';
}


void createDataArray(directiveType type, void **dataArray, int numOfVariables, const char *content, int index)
{
    /* '.db'/'.asciz' directives contain 1 byte variables */
    if(type == DB || type == ASCIZ)
        *dataArray = callocWithCheck(sizeof(char) * numOfVariables);
    /* '.dh' directive contain 2 bytes variables */
    else if(type == DH)
        *dataArray = callocWithCheck(sizeof(short) * numOfVariables);
    /* '.dw' directive contain 4 bytes variables */
    else if(type == DW)
        *dataArray = callocWithCheck(sizeof(int) * numOfVariables);

    if(type == DH || type == DB || type == DW)
        scanDVariableToArray(content, index, type, *dataArray);

     /* The scan for ".asciz" directive different from the other directive types scan */
    else if(type == ASCIZ)
        scanAscizVariableToArray(content, index, *dataArray);
}


void checkAscizCharValidity(newLine *line, int contentIndex, int *numOfVariables, bool inQuotes)
{
    /* If there is a not-white-space character outside the quotes */
    if(!inQuotes && !isWhiteSpace(line->content[contentIndex]))
        line -> error = addError("The string is not bounded by quotes");

    if(inQuotes)
    {
        /* If char is in quotes and isn't printable. */
        if(!isprint(line->content[contentIndex]))
            line->error = addError("String contain char that cannot be printed");
        else
            (*numOfVariables)++;
    }
}


void checkAscizDirectiveLine(newLine *line, int contentIndex, int *numOfVariables)
{
    /* Indicates whether the current character is in or out of quotes. */
    bool inQuotes = FALSE;

    /* Checks the validity on each character in the input line */
    for(; line -> content[contentIndex] != '\n' && currentState(line) == VALID; contentIndex++)
    {
		if(line->content[contentIndex] == '\"')
			inQuotes = !inQuotes;
		else
			checkAscizCharValidity(line, contentIndex, numOfVariables, inQuotes);

    }
    /* If the line ends with open quotes */
    if(currentState(line) == VALID && inQuotes && line->content[contentIndex] == '\n')
        line -> error = addError("The string is not bounded by quotes");

    /* If no error was found, increases number of variables by one for '\0' character */
    if(currentState(line) == VALID)
        (*numOfVariables)++;
}


void checkDTypeDirectiveLine(newLine *line, directiveType thisDirective, int contentIndex, int *numOfVariables)
{
    /* While no error is found, check line's validation. */
    while (currentState(line) == VALID && line->content[contentIndex] != '\n')
    {
        /* If comma is missing or appearing in wrong place. */
        if(checkForComma(line, &contentIndex, *numOfVariables) == INVALID)
            break;
        if(thisDirective == DH)
			checkInteger(line, &contentIndex, numOfVariables, max2BytesIntLength, max2BytesIntVal, min2BytesIntVal);
        else if(thisDirective == DW)
			checkInteger(line, &contentIndex, numOfVariables, max4BytesIntLength, max4BytesIntVal, min4BytesIntVal);
        else if(thisDirective == DB)
			checkInteger(line, &contentIndex, numOfVariables, max1ByteIntLength, max1ByteIntVal, min1ByteIntVal);
    }
}


void checkDirectiveByType(newLine *line, directiveType type, int index, int *numOfVariables)
{
    if(type == DH || type == DW || type == DB)
         checkDTypeDirectiveLine(line, type, index, numOfVariables);

    else if(type == ASCIZ)
         checkAscizDirectiveLine(line, index, numOfVariables);
}


state dataStorageDirective(newLine *line, directiveType type, int index, int *numOfVariables, void **dataArray)
{
    /* If no operands appear after the directive word. */
    if(emptyLine(line->content, index))
        line-> error = addError("Missing Operands");

    else if (line->content[index] != ' ' && line->content[index] != '\t')
        line->error = addError("No spacing between the directive word and the first operand");

    /* If no error was found, executing Specific check of syntax and operands of the directive. */
    else
        checkDirectiveByType(line, type, index, numOfVariables);

	/* If directive line is valid, Produces an array that contains the variables that appears in the line */
    if(currentState(line) == VALID)
        createDataArray(type, dataArray, *numOfVariables, line->content, index);

    return currentState(line);
}

/* Process an input line that represent data storage directive */
void processDataStorageDirective(char *label, newLine *line, directiveType type, bool labelSet, int index, symbolTable *symTable, dataTable *dataImage, long *DC)
{
    int numOfDataVariables = 0; /* The number of variables the current directive accepts  */
    void *dataArray = NULL;

    /* Checks if the line syntax and operands are valid according to the directive dataType */
    if(dataStorageDirective(line, type, index, &numOfDataVariables, &dataArray) == VALID)
    {
        /* If there is a label in the start of the line that isn't defined, add it to the symbol table */
        if(labelSet == TRUE)
        {
            if(labelIsDefined(label, line, *symTable, data) == FALSE)
                addToSymbolTable(symTable, label, *DC, data);
        }
        /* Adds the received data to the data image linked list and continues to the next line.*/
		addToDataImage(type, numOfDataVariables, DC, dataArray, dataImage);
    }
}


void processDirective(directiveWord *directToken, bool labelSet, newLine *line, int *index, long *DC, symbolTable *symTab, dataTable *dImage, char *label)
{
    if(searchDirective(line, directToken) == VALID)
    {
        /* Checks if the directive word is '.dh'/ '.dw'/ '.db'/ '.asciz' */
        if(isDataStorageDirective(directToken->type))
            processDataStorageDirective(label, line, directToken->type, labelSet, *index, symTab, dImage, DC);

        /* If the directive is '.entry' skip it (this directive will be handled in the second pass)  */
        if(directToken->type == EXTERN)
        {
            getLabelName(line->content, index, label);
            /*If the label isn't defined yet, add it to the symbol table as 'extern' */
            if(labelIsValid(line,label) && !labelIsDefined(label, line, *symTab, external))
                addToSymbolTable(symTab, label, 0, external);
        }
    }
}