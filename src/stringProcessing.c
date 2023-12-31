
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../include/stringProcessing.h"
#include "../include/general.h"


bool isAlphanumeric(const char *symbol)
{
    int i;
    unsigned long symbolLength = strlen(symbol);
    for(i = 0; i < symbolLength; i++)
    {
        if(!isalpha(symbol[i]) && !isdigit(symbol[i]))
			return FALSE;
    }
    return  TRUE;
}


state checkForComma(newLine *line, int *index, int numOfVariables)
{
    skipSpaces(line->content, index);

    if(line->content[*index] == ',')  /* checks for multiple commas or if comma located in an invalid location */
    {
        (*index)++;
        skipSpaces(line->content, index);

        if(line->content[*index] == ',')
            line -> error = addError("multiple commas");

        /* If the comma located before the first variable or after the last variable */
        else if(numOfVariables == 0 || line->content[*index] == '\n')
            line->error = addError("A comma appears before the first variable or after the last variable");
    }
    /* If the current character is not a comma, check if we are between numbers */
    else if(numOfVariables != 0 && line->content[*index] != '\n')
        line-> error = addError("Missing comma");
    return currentState(line);
}


void checkInteger(newLine* line, int* contentIndex, int* numOfVariables, int maxNumLength, int maxVal, int minVal)
{
    int numValue;
    char numString[maxNumLength + 2];

    numValue = scanInt(line->content, contentIndex, numString, maxNumLength);

    if(!intIsValid(numString, numValue, maxVal, minVal))
        line->error = addError("Invalid operand, Operand must be an integer in the range defined by the instruction/directive");
    else
        (*numOfVariables)++;
}


void checkRegister(newLine *line, int *contentIndex, int *numOfScannedOperands)
{
    int registerNum;
    char registerString[maxRegisterLength + 2];
    state registerState = VALID;

    /* Check that the first char of the current word is '$' and the second is (or a start of) a number */
    if(line->content[*contentIndex] != '$' || !isdigit(line->content[(*contentIndex) + 1]))
        registerState = INVALID;
    else
    {
        (*contentIndex)++; /* skips to the start of the operand number */
        registerNum = scanInt(line->content, contentIndex, registerString, maxRegisterLength);
        if(!intIsValid(registerString, registerNum, maxRegister, minRegister))
            registerState = INVALID;
    }
    if(registerState == INVALID)
        line->error = addError("Invalid register. register must start with '$' and represent a number between 0-31");
    else
        (*numOfScannedOperands)++;
}


void checkOperandsAmount(newLine* line, unsigned int opcode, int numOfScannedOperands, bool endOfScan)
{
	bool threeOperandsError, twoOperandsError, oneOperandError;

	threeOperandsError = (!endOfScan && numOfScannedOperands > 3) || (endOfScan && numOfScannedOperands != 3);
	twoOperandsError = (!endOfScan && numOfScannedOperands > 2) || (endOfScan && numOfScannedOperands != 2);
	oneOperandError = (!endOfScan && numOfScannedOperands > 1) || (endOfScan && numOfScannedOperands != 1);

    if(opcode == 0 && threeOperandsError)
        line->error = addError("Incorrect number of registers.'R' arithmetic and logical instructions should receive 3 registers");

    else if(opcode == 1 && twoOperandsError)
        line->error = addError("Incorrect number of registers.'R' copy instructions should receive 2 registers");

    else if(((opcode >= 10 && opcode <= 14) || (opcode >= 19 && opcode <= 24)) && threeOperandsError)
        line->error = addError("Incorrect number of operands.'I' copy/loading/saving memory instructions should receive 3 operands");

    else if((opcode >= 15 && opcode <= 18) && threeOperandsError)
        line->error = addError("Incorrect number of operands.'I' Conditional branching instructions should receive 3 operands");

    else if((opcode == 30 || opcode == 31 || opcode == 32) && oneOperandError)
        line->error = addError("Incorrect number of operands, jump instruction should receive only one operand");
}


int get2BytesInt(const char *content, int *index)
{
    int intValue;
    char intString[max2BytesIntLength + 2];

    intValue = scanInt(content, index, intString, max2BytesIntLength);

    return intValue;
}


int getRegister(const char *content, int *index)
{
    int registerNum;
    char registerString[maxRegisterLength + 2];

    for(; content[*index] != '$'; (*index)++ );  /* Continues until encountering the beginning of a register */

    (*index)++;  /* Skips to the register number */

    registerNum = scanInt(content, index, registerString, maxRegisterLength);

    return registerNum;
}


bool isRegister(const char *content, int index)
{
    /* Continues until encountering beginning of an operand */
    for(; isWhiteSpace(content[index]) || content[index] == ','; index++);
    /* If encountered beginning of a register */
    if(content[index] == '$')
        return TRUE;
    return FALSE;
}


int scanInt(const char *content, int *contentIndex, char *numString, int maxLength)
{
    int numValue;
    int i = 0;

    skipSpaces(content, contentIndex);

    /* Scans the string until encounter separation - space, comma or end of line */
    while(!isspace(content[*contentIndex]) && content[*contentIndex] != ',' && i <= maxLength)
        numString[i++] = (char)(content[(*contentIndex)++] + '\0');

	numString[i] = '\0'; /* End of string */
	numValue = atoi(numString);

    return numValue;
}


bool intIsValid(char* numString, int numValue, int maxValue, int minValue)
{
    int i = 0;
    bool isValid = TRUE;

	/* If the first character in 'numString' is a valid mathematical sign */
	if(numString[i] == '-' || numString[i] == '+')
		i++;

    for(; i < strlen(numString) && isValid == TRUE; i++)
    {
        /* Checks if all the characters of the string that represent the integer are number or mathematical symbols */
        if(!isdigit(numString[i]))
            isValid = FALSE;
    }

	/* Checks if the integer is in the proper limits */
	if(isValid == TRUE && numValue > maxValue || numValue < minValue)
		isValid = FALSE;
    return isValid;
}







