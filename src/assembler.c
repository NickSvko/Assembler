/*
 * Author: Nikita Savchenko
 * Description: Converts files written in imaginary assembly language into machine code files.
 */

#include "../include/files.h"
#include "../include/createOutputFiles.h"


/*
   Function: assembler
   Description:
   Processes each input file separately. For each valid file, produces relevant output files.
   If an input file is invalid, prints the encountered errors.
   Parameters:
    - argc: Number of command line arguments.
    - argv: Array of command line argument strings.
   Returns:
    - int: Returns 0 on completion.
   Notes:
     - Processes between 1 and 3 input files received from the command line,
       written in a fictional assembly language, treating each file individually.
     - For each valid input file, generates 1-3 files based on the content:
        - Entries File: Contains file details,
          explained within the 'exportToExternAndEntryFiles' function in the 'createFiles.c' file.
        - Externals File: Contains file details,
          explained within the 'exportToExtAndEntFiles' function in the 'createFiles.c' file.
        - Object File: Represents instructions and directives from the input file in hexadecimal format.
          Additional information is available in the 'createObjectFile' function within the 'createFiles.c' file.
    - If an input file is invalid, prints encountered errors (one per invalid line) without generating any output files.
 */
int assembler(int argc, char *argv[])
{
    int i;
    long ICF, DCF;
    codeTable codeImage = NULL;
    dataTable dataImage = NULL;
    attributesTable attributesTab = NULL;

    if (argc < 2)
    {
        fprintf(stderr, "Error: Missing input files.\n");
        return 0;
    }

    for (i = 1; i < argc; ++i)
    {
        if (fileIsValid(argv[i], &codeImage, &dataImage, &attributesTab, &ICF, &DCF))
            createOutputFiles(argv[i], codeImage, dataImage, attributesTab, ICF, DCF);
    }
    return 0;
}


int main(int argc, char *argv[])
{
    return assembler(argc, argv);
}