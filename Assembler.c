
/*
 *
 *
 * * Course No. 20465- Final Project
 * Student:  Francois Oren Chikli
 *
 * l
 FINAL PROJECT
===========

Assembler.c

Short Description:
------------------
A file containing several function for analysing a a given file.
The output can be a list of errors found in its assembly syntax, or the creation of output files corresponding
 to the syntax.

Assumptions:
A valid file pointer of an "file.as" file is given.

functions:
AssemblerActivate(),GenerateOutFiles(),print_weird()




----------------------------------------

 */

#include "GlobalDeclarations.h"
#include "FirstPass.h"
#include "SecondPass.h"


#include "ErrorList.h"
#include "InstructionsList.h"
#include "SymbolTable.h"
#include "FirstPassList.h"
#include "ExternList.h"
#include "EntryList.h"
#include "DataList.h"


#include "FileFunctions.h"




/*
 AssemblerActivate()
-----------------
Get a file pointer to the current analysed file and its name.
The method than scans the file with the FirstPass Method, checks if there are no errors via the haserror() method.
If no errors are found,  the SecondPas() methode is called, and it it retuns a Succses Flag, the  GenerateOutFiles()
 method is called, which created the output files
 If an error concerning the assembler code is detected the method outputs FALSE, or TRUE Other wise..


 * */
int AssemblerActivate(FILE *fp, char *fileName)
{
    int ReturnFlag=TRUE; /*Create a flag integer for to be used after activating various methods, and to help determine
 *                    what to do next*/
    int errorFlag;/*a flag for the type of errors found- if an ERROR is found the progam will stop the analysis*/

    int IC=INITIAL_IC; /*Set the Instructions and Data counters to the initial user defined values*/
    int DC=INITIAL_DC;

     FirstPass(fp, &IC, &DC); /*Scan the file and modiffies the IC and DC that were given to it- affter the execution
 *                            the IC and DC changed as the number of Instructions and Data words are created*/



    if((errorFlag=hasError())!=NOTHING) /*The method detects if the errors list is empty or any error messages (warning mesages are
 *                        allowed) */
    {
        printErrorList(fileName);/*Display the errors*/

        if (errorFlag==ERROR) /*If the errors list contained any errors and not only Warnings*/
        {
            freeFirstPasslist(); /*The row in in valid, thus the list needs to be freed */
            freeErrorList(); /*frees the errors list since the scan wont continue*/
            ReturnFlag=FALSE;
        }
    }

    if (errorFlag!=ERROR) /*If there are no errors or only warnings*/
    {

        if (SecondPass(IC, DC) == TRUE) /*Preform a second pass as described in the main*/
        {
            GenerateOutFiles(IC, DC, fileName); /*Create the output files for the given file*/

        }
        else /*There was an error during the second pass*/
        {
            printErrorList(fileName);/*Display the errors*/
            freeErrorList(); /*frees the errors list*/
            ReturnFlag=FALSE;
        }
    }

    /*free all the lists and data structures used for the next iteration*/
    freeInstructionslist();
    freeDataList();
    FreeSymbolTable();
    freeEntrylist();
    freeExternList();

    return ReturnFlag;
}


