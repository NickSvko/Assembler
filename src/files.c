#include <string.h>
#include <stdlib.h>
#include "../include/firstPass.h"
#include "../include/secondPass.h"
#include "../include/tables.h"
#include "../include/general.h"


state openFile(FILE** fileDescriptor, const char* fileName, char* mode)
{
	state fileOpening = VALID;

	/* Opens the file according to the received mode - reading/writing */
	if (strcmp("r", mode) == 0)
		*fileDescriptor = fopen(fileName, "r");
	else if (strcmp("w", mode) == 0)
		*fileDescriptor = fopen(fileName, "w");

	if (*fileDescriptor == NULL)
	{
		if (strcmp("r", mode) == 0)
			fprintf(stderr, "Error: failed to access file '%s' for reading.\n", fileName);
		else if (strcmp("w", mode) == 0)
			fprintf(stderr, "Error: failed to access file '%s' for writing.\n", fileName);

		fileOpening = INVALID;
	}
	return fileOpening;
}


void updateValues(long* ICF, long* DCF, long IC, long DC, symbolTable symTab, dataTable dataTab)
{
	dataTable dataEntry;
	symbolTable symbolEntry;

	/* Preserves the final values of IC and DC */
	*ICF = IC;
	*DCF = DC;

	/* Updates the addresses of all data entries in the data image table by incrementing each address with ICF value. */
	for (symbolEntry = symTab; symbolEntry != NULL; symbolEntry = symbolEntry->next)
	{
		if (symbolEntry->type == data)
			symbolEntry->value += (*ICF);
	}

	/* Updates in the data Image the addresses of all the data by adding the ICF dataType to each dataType */
	for (dataEntry = dataTab; dataEntry != NULL; dataEntry = dataEntry->next)
		dataEntry->address += (*ICF);

}


state processFile(codeTable* codeImage, dataTable* dataImage, attributesTable* attrTab, long* ICF, long* DCF, FILE* fd,
		newLine* line)
{
	state fileState = INVALID;
	symbolTable symbolTab = NULL; /* represents the symbol table */
	long instructionCounter = ICInitialValue, dataCounter = 0;

	/* If the first pass succeeded, updates the required values and proceed to the second pass */
	if (firstPass(line, &instructionCounter, &dataCounter, &symbolTab, codeImage, dataImage, fd) == VALID)
	{
		updateValues(ICF, DCF, instructionCounter, dataCounter, symbolTab, *dataImage);
		rewind(fd);  /* Starts from beginning of file again */
		if (secondPass(line, symbolTab, *codeImage, attrTab, fd) == VALID)
			fileState = VALID;
	}



	freeSymbolTable(symbolTab);
	fclose(fd);
	/* Returns whether the two-passes succeeded or not */
	return fileState;
}


char* getFileFullName(char* fileName, char* fileExtension)
{
	char *fullName, *temp;
	fullName = callocWithCheck(strlen(fileName) + strlen(fileExtension) + 1);
	temp = strtok(fileName, "."); /* Copy the filename without the extension */
	strcpy(fullName, temp);
	strcat(fullName, fileExtension);
	return fullName;
}


state fileIsValid(const char* fileName, codeTable* codeImage, dataTable* dataImage, attributesTable* attrTab, long* ICF,
		long* DCF)
{
	state fileState = INVALID;
	FILE* fileDescriptor;
	newLine* line = (newLine*)callocWithCheck(sizeof(newLine));

	line->sourceFileName = fileName;

	/* If the file opening succeeded, continue with the file's processing */
	if (openFile(&fileDescriptor, line->sourceFileName, "r") == VALID)
		fileState = processFile(codeImage, dataImage, attrTab, ICF, DCF, fileDescriptor, line);

	free(line);
	return fileState;
}