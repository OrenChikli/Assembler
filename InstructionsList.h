/*
 * InstructionsList.h:
 * ----------------------------------------------------
 * This is the header file for the InstructionsList data structure.
 * This is a linked list storing an assembly instruction type word binary code representation at each node,
 * along with it address and other identifying features.
 *
 * The declarations here are for external usage by other methodes.
 *
 *
 *
 * */

#ifndef PROJECT_WORDSLIST_H
#define PROJECT_WORDSLIST_H
#include "GlobalDeclarations.h"



 enum ADDRESSING  {IMMEDIATE_ADDRESING,DIRECT_ADDRESING,JUMP_ADDRESING,REGISTER_ADDRESING,NIL_ADDRESING};
 /*an enumeration of the addressing method for assembly words as described in p17-18 of the instructions manual
  * e.g IMMEDIATE_ADDRESING is 0 as specified in the manual, and so on*/

 enum JUMP_PARAMETER_ADDRESSING  {IMMEDIATE_JMP,LABEL_JMP,NIL_JMP,REGISTER_JMP};
 /*an enumeration of the addressing methods for the parameter used in jump instructions, as spesifeid in p16 of
  * the instructions manual*/

typedef struct wnode * instructions_list_ptr; /*a definition of a pointer to a InstructionsList node*/

void insertCommandToWordList(int command, char *SourceOperand, int SourceFlag, int returnValueSource, char *DestOperand,
                             int DestFlag, int returnValueDest, int LineNum, int IC);
/*A method for inserting a non jump type command into the list, given the appropriate flags.*/

void insetJumpCommand(int commandNum, char *DestOperand, int returnValueDest, int param1, int param1_flag,
        char *param1_char, int param2, int param2_flag, char *param2_char, int LineNum, int IC);
/*A method for inserting a  jump type command into the list, given the appropriate flags.*/



void returnInstructionsHead(instructions_list_ptr *node); /*a method for returning the head of the list*/

unsigned short int * get_Word_Binary_Value(instructions_list_ptr node);
/*a method used by the GenerateOutFiles() method in the FileFunction.c file for getting the numerical value of the
 * binary assembler word representation from the list */

instructions_list_ptr getNextInstruction(instructions_list_ptr node);
/*a method for returning the next node to the given node, used by the GenerateOutFiles() method in the FileFunction.c*/

int getInstructionAddress(instructions_list_ptr node); /*a method for getting the addrdess of the given instruction
 * node*/


void freeInstructionslist(); /*a method for freeing the list when it is not needed anymore*/

#endif 
