
#include <string.h>
#include "../include/stringProcessing.h"
#include "../include/labels.h"
#include "../include/general.h"
#include "../include/tables.h"
#include "../include/lineHandling.h"


instructionWord *getReservedInstructions(int *numberOfInstructions)
{
    const  unsigned int noFunct = -1;

    static instructionWord instruction[] = {
            {"add",  0,  1, R}, {"sub", 0, 2, R},
            {"and",  0,  3, R}, {"or", 0, 4, R},
            {"nor",  0,  5, R}, {"move", 1, 1, R},
            {"mvhi", 1,  2, R}, {"mvlo", 1, 3, R},
            {"addi", 10, noFunct, I}, {"subi", 11, noFunct, I},
            {"andi", 12, noFunct, I}, {"ori", 13, noFunct, I},
            {"nori", 14, noFunct, I}, {"bne", 15, noFunct, I},
            {"beq",  16, noFunct, I}, {"blt", 17, noFunct, I},
            {"bgt",  18, noFunct, I}, {"lb", 19, noFunct, I},
            {"sb",   20, noFunct, I}, {"lw", 21, noFunct, I},
            {"sw",   22, noFunct, I}, {"lh", 23, noFunct, I},
            {"sh",   24, noFunct, I}, {"jmp", 30, noFunct, J},
            {"la",   31, noFunct, J}, {"call", 32, noFunct, J},
            {"stop", 63, noFunct, J}};

    *numberOfInstructions = sizeof(instruction) / sizeof(instruction[0]);
    return instruction;
}


state searchInstruction(instructionWord *instructionToken)
{
    /* numberOfInstructions = The total number of the available instructions */
    int i,numberOfInstructions;
    instructionWord *instruction = getReservedInstructions(&numberOfInstructions);

    /* Searches for a match between the current word and one of the instructions name */
    for(i = 0; i < numberOfInstructions; i++)
    {
        /* if a match is found saves the matched instruction values and stops */
        if(strcmp(instructionToken->name, instruction[i].name) == 0)
        {
            instructionToken->type = instruction[i].type;
            instructionToken->opcode = instruction[i].opcode;

            if(instructionToken->type == R)
                instructionToken->funct = instruction[i].funct;
            break;
        }
    }
    /* If no match is found after iterating all the instructions  */
    if(i == numberOfInstructions)
        return INVALID;
    return VALID;
}


state instructionWordState(newLine *line, instructionWord *instructionToken, int *index)
{
    int i = 0;

    skipSpaces(line->content, index);

    while(!isWhiteSpace(line->content[*index]) && i <= maxInstructionLength) /* Copies the current word */
        instructionToken->name[i++] = line->content[(*index)++];

    /* If the current word is too long to be an instruction */
    if(i == maxInstructionLength)
        line->error = addError("Invalid instruction name");
    /* End of string */
    else
        instructionToken->name[i] = '\0';

    if(currentState(line) == VALID && searchInstruction(instructionToken) == INVALID)
        line->error = addError("Invalid instruction name");

    return currentState(line);
}


void checkJOperandsSyntax(newLine *line, unsigned int opcode, char *symbol, int *index, int *numOfScannedOperands)
{
    /* If 'jump' instructions with register operand */
    if(opcode == 30  && line->content[*index] == '$')
        checkRegister(line, index, numOfScannedOperands);
    else
    {
        getLabelName(line->content, index, symbol);
        labelIsValid(line, symbol);
        (*numOfScannedOperands)++;
    }
}


void checkIOperandsSyntax(newLine *line, unsigned int opcode, char *symbol, int *index, int *numOfScannedOperands)
{
    /* dataType 'I' copy instructions or dataType 'I'  loading and saving memory instructions */
    if((opcode >= 10 && opcode <= 14) || (opcode >= 19 && opcode <= 24))
    {
        /* THE second operand in line must be an integer */
        if((*numOfScannedOperands) == 1)
			checkInteger(line, index, numOfScannedOperands, max2BytesIntLength, max2BytesIntVal, min2BytesIntVal);
        /* The first and third operands in line must be registers */
        else
            checkRegister(line, index, numOfScannedOperands);
    }
    /* Type 'I' Conditional branching instructions */
    else if(opcode >= 15 && opcode <= 18)
    {
        /* The third operand must be a label */
        if((*numOfScannedOperands) == 2)
        {
            getLabelName(line->content, index, symbol);
            labelIsValid(line, symbol);
            (*numOfScannedOperands)++;
        }
        /* The first and second operands in line must be registers */
        else
            checkRegister(line, index, numOfScannedOperands);
    }
}


void checkOperandByType(newLine *line, unsigned int opcode, char *symbol, int *index, int *numOfScannedOperands)
{
    /* Type 'R' instructions */
    if(opcode == 0 || opcode == 1)
        checkRegister(line, index, numOfScannedOperands);

    /* Type 'I' instructions */
    else if((opcode >= 10 &&  opcode <= 24))
        checkIOperandsSyntax(line, opcode, symbol, index, numOfScannedOperands);

    /* Type 'J' instructions - without 'stop' */
    else if(opcode == 30 || opcode == 31 || opcode == 32)
        checkJOperandsSyntax(line, opcode, symbol, index, numOfScannedOperands);
}


void checkInstructionSyntax(newLine *line, unsigned int opcode, int contentIndex)
{
    int numOfScannedOperands = 0;
    char symbol[maxLabelLength + 1] = {0};

    /* 'stop' instruction validity check */
    if(opcode == 63 && !emptyLine(line->content, contentIndex))
        line->error = addError("Excessive text after 'stop' instruction");

    while(currentState(line) == VALID && line->content[contentIndex] != '\n')
    {
        if(checkForComma(line,&contentIndex, numOfScannedOperands) == VALID)
            checkOperandByType(line, opcode, symbol, &contentIndex, &numOfScannedOperands);

		skipSpaces(line->content, &contentIndex);

		if(currentState(line) == VALID)
		{
			if(line->content[contentIndex] != '\n')
				checkOperandsAmount(line, opcode, numOfScannedOperands, FALSE);
			if(line->content[contentIndex] == '\n')
				checkOperandsAmount(line, opcode, numOfScannedOperands, TRUE);
		}
    }
}


state instructionLineState(newLine *line, instructionWord instructionToken, int contentIndex)
{
    /* If there is no operands after the instruction , and it's not 'stop' instruction */
    if(instructionToken.opcode != 63 && emptyLine(line->content, contentIndex))
        line-> error = addError("Missing Operands");

    /* If there is no spacing between the instruction and the first operand, and it's not 'stop' instruction */
    else if (instructionToken.opcode != 63 && line->content[contentIndex] != ' ' && line->content[contentIndex] != '\t')
        line->error = addError("No spacing between the instruction word and the first operand");

    /* If no error was found, executing syntax and operands check for the instruction dataType */
    else
        checkInstructionSyntax(line, instructionToken.opcode, contentIndex);

    return currentState(line);
}


void getInstruction(const char *content, int *contentIndex, instructionWord *instructionToken)
{
    int i = 0;

    skipSpaces(content, contentIndex);

    /* Copies the instruction */
    while(!isWhiteSpace(content[*contentIndex]))
        instructionToken->name[i++] = content[(*contentIndex)++];

    /* Finds the current instruction and save its opcode and dataType in the instructionToken */
    searchInstruction(instructionToken);
}


state addressState(newLine *line, symbolTable label, instructionType type, long address)
{
    if(type == I)
    {
        /* label can't be defined as external in type 'I' instruction */
        if(label->isExternal)
            line->error = addError("Label can't be defined as external in 'I' conditional branching instruction");
        /* Address must be in the 16-bit range */
        else if(address < min2BytesIntVal || address > max2BytesIntVal)
            line->error = addError("The label address isn't in the correct range for type 'I' instruction");
    }
    /* Address must be in the 25-bit range */
    else if(type == J && (address < min25BitsIntVal || address > max25bitsIntVal))
        line->error = addError("The label address isn't in the correct range for type 'J' instruction");

    return currentState(line);
}


state getAddress(newLine *line, long instructionAddress, symbolTable label, instructionType type, long *address)
{
    /* In 'I' dataType instruction, The 'immed' field contains the distance between the label and the instruction */
    if(type == I)
        (*address) = label->value - instructionAddress;

    /* In 'J' dataType instruction, the 'distance' represent the required label's dataType */
    else if(type == J)
    {
        /* If label set as external, labels dataType unknown */
        if(label->isExternal)
            (*address) = 0;
        else
            (*address) = label->value;
    }
    return addressState(line, label, type, *address);
}


void processInstruction(newLine *line, int *index, bool labelSet , char *label, symbolTable *symTab, codeTable *cImage, long *IC)
{
    instructionWord instructionToken;
    if(currentState(line) == VALID && instructionWordState(line, &instructionToken, index) == VALID)
    {
        if(labelSet && labelIsValid(line,label) && !labelIsDefined(label, line, *symTab, code))
            addToSymbolTable(symTab, label, *IC, code);

        if(currentState(line) == VALID && instructionLineState(line, instructionToken, *index) == VALID)
			addToCodeImage(line->content, *index, instructionToken, cImage, IC, line->number);
    }
}


bool instructionWithLabelOperand(newLine *line, int index, unsigned int opcode)
{
    if((opcode >= 15 && opcode <= 18) || (opcode >= 30 && opcode <= 32) && !(opcode == 30 && isRegister(line->content, index)))
        return TRUE;
    return FALSE;
}




