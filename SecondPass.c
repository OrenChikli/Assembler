/*
 *
 * * Course No. 20465- Final Project
 * Student:  Francois Oren Chikli
 *
 FINAL PROJECT
===========

first_pass_list.c

Short Description:
------------------
A file defining the first_pass_list data structure, which is a singly liked list for containing flags of commands after
 the first pass on the list. IF the first pass does not produce any errors, this list is read during the second pass
 and traslated into the instructions list (InstructionsList.c) which contains the words of the assembly code in a way
 that can later be printed in the needed binary representation.

A node is a union between 2 structs- 1 for a normal command, and the other for a jump type command

  * The insertion to this list is doen in a non changing order, meaning each nodes location is final and
 * thus a Tail node is created for better time coplexity insertion at the end of the list

----------------------------------------
 */


#include "GlobalDeclarations.h"
#include "FirstPassList.h"
#include "DataList.h"
#include "ErrorList.h"
#include "SymbolTable.h"
#include "FirstPass.h"
#include "FirstPassHandlers.h"
#include "ExternList.h"


#define MAX_FILE_SIZE 900 /*The Maximum allowed lines in the final .ob files*/



/*
SecondPass()
-----------------
The method is  activated after a successful first pass scan of the file via the FirstPass() method defined in the
FirstPass.c file.

The method is given the finall IC and DC of the scan, checks if it does not exceed the lentgth limitation for
 file creation and creates the InstructionsList data structure, defined in the InstructionList.c file via the
 method from_fplist_to_wordslist().
 At the end, an InstructionsList data structure is created for further use by the program and the firstPasslist
 (defined in the firstPassList.c file) is freeed.

 Assumptions:
 A succsesful activation of the FirstPass() method and the creation of the FirstPassList data structure.
 * */


int SecondPass(int IC, int DC)
{
    int ReturnFlag=TRUE;



    if ((IC-INITIAL_IC+DC-INITIAL_DC)>MAX_FILE_SIZE) /*The number of lines exedess the allowed maximum limit*/
    {
        insertToErrorList("The file size exceeded memory limits for translation", 0, ERROR);
        ReturnFlag=FALSE;

    }
    else
    {

        from_fplist_to_wordslist();
        /*A method to create the instructions list to be finally exported. This is an implementation of the second pass:
*            at first a representation of the file was created with the first_pass list and now it is scanned again
*                completing the list*/
    }

    freeFirstPasslist(); /*The first_pass_list in not needed anymore thus it is freed*/


return ReturnFlag;
}
